/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QSplitter>
#include <QBoxLayout>
#include <QAction>

#include "kndropproxycontainer.h"
#include "knglobal.h"

#include "knmusicsearchbase.h"
#include "knmusicgenremodel.h"
#include "knmusiccategorydisplay.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorylistviewbase.h"
#include "knmusicglobal.h"
#include "knmusicsolomenubase.h"

#include "knmusiclibrarygenretab.h"

KNMusicLibraryGenreTab::KNMusicLibraryGenreTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_dropProxy=new KNDropProxyContainer(viewerWidget());
    setContentWidget(m_dropProxy);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyShow,
            this, &KNMusicLibraryGenreTab::onActionTabShow);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyHide,
            this, &KNMusicLibraryGenreTab::onActionTabHide);

    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);

    //Initial the main splitter.
    m_splitter=new QSplitter(m_dropProxy);
    m_splitter->setHandleWidth(0); //This is beautiful.
    m_splitter->setChildrenCollapsible(false);
    //Set the proxy.
    m_dropProxy->setFocusProxy(m_splitter);
    mainLayout->addWidget(m_splitter);

    //Initial the list.
    m_genreList=new KNMusicCategoryListViewBase(m_dropProxy);
    m_dropProxy->setFocusProxy(m_genreList);
    m_splitter->addWidget(m_genreList);

    //Initial the category display.
    m_genreDisplay=new KNMusicCategoryDisplay(m_dropProxy);
    m_genreDisplay->setMusicTab(this);
    m_genreDisplay->setCategoryColumn(Genre);
    m_splitter->addWidget(m_genreDisplay);

    //Set viewer properties after add widgets.
    m_splitter->setCollapsible(1, false);
    m_splitter->setStretchFactor(1, 1);

    //Initial the show in and search action.
    initialShowInAction();
    initialFindAction();

    //Connect retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicLibraryGenreTab::retranslate);
    //Retranslate.
    retranslate();
}

QAction *KNMusicLibraryGenreTab::showInAction()
{
    return m_showInGenre;
}

QString KNMusicLibraryGenreTab::caption()
{
    return tr("Genres");
}

QPixmap KNMusicLibraryGenreTab::icon()
{
    return QPixmap(":/plugin/music/category/04_genres.png");
}

void KNMusicLibraryGenreTab::showInTab(const KNMusicDetailInfo &detailInfo)
{
    //Clear the search result.
    KNMusicGlobal::musicSearch()->search("");
    //Get the row of the file.
    int musicRow=m_musicLibrary->rowFromDetailInfo(detailInfo);
    //If the row is available.
    if(musicRow!=-1)
    {
        //Get the genre name of the row.
        QModelIndex categoryIndex=
                proxyCategoryModel()->categoryIndex(
                    m_musicLibrary->itemText(musicRow,
                                             m_categoryModel->categoryIndex()));
        //Check is the catgeory vaild.
        if(categoryIndex.isValid())
        {
            //Change the current category index.
            m_genreList->setCurrentIndex(categoryIndex);
            m_genreList->scrollTo(categoryIndex,
                                  QAbstractItemView::PositionAtCenter);
            //Set the details to display the index of the song.
            m_genreDisplay->scrollToSourceRow(musicRow);
            //Ask to show the genre tab.
            emit requireShowTab();
        }
    }
}

void KNMusicLibraryGenreTab::retranslate()
{
    //Update the goto caption.
    m_showInGenre->setText(tr("Go to Genre"));
    //Check the category model.
    if(m_categoryModel!=nullptr)
    {
        //Set the no category text.
        m_categoryModel->setNoCategoryText(tr("No Genre"));
        //Update the detail info, might update the translation.
        if(m_genreList->currentIndex().isValid())
        {
            onActionCategoryIndexChanged(m_genreList->currentIndex());
        }
    }
}

void KNMusicLibraryGenreTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Do original set library model.
    KNMusicLibraryCategoryTab::setLibraryModel(model);
    //Save the library.
    m_musicLibrary=model;
    //Do connections.
    connect(m_dropProxy, &KNDropProxyContainer::requireAnalysisFiles,
            m_musicLibrary, &KNMusicLibraryModel::addFiles);
    //Set the model.
    m_genreDisplay->setLibraryModel(m_musicLibrary);
}

void KNMusicLibraryGenreTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Save the model pointer.
    m_categoryModel=static_cast<KNMusicGenreModel *>(model);
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(m_categoryModel);
    //Update the no category text.
    retranslate();

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to tree view.
    m_genreList->setModel(proxyCategoryModel());
    proxyCategoryModel()->sort(0, Qt::AscendingOrder);
    connect(m_genreList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicLibraryGenreTab::onActionCategoryIndexChanged);
}

void KNMusicLibraryGenreTab::onActionSearch(const QString &text)
{
    //Search in genres.
    proxyCategoryModel()->setFilterFixedString(text);
    //We want to track the result index.
    QModelIndex resultIndex=m_genreList->currentIndex();
    //Check whether there's select item in the treeview.
    if(!resultIndex.isValid())
    {
        //Check it's there any other choice of the index.
        if(proxyCategoryModel()->rowCount()>0)
        {
            //New category index.
            resultIndex=proxyCategoryModel()->index(0,0);
            //Set the current index to the first item of the category model.
            m_genreList->setCurrentIndex(resultIndex);
        }
        else
        {
            return;
        }
    }
    //Trace the result index.
    m_genreList->scrollTo(resultIndex,
                           QAbstractItemView::PositionAtCenter);
}

void KNMusicLibraryGenreTab::onActionCategoryIndexChanged(const QModelIndex &index)
{
    QModelIndex categoryIndex=proxyCategoryModel()->mapToSource(index);
    //Check is the no category item.
    if(categoryIndex.row()==0)
    {
        m_genreDisplay->showNoCategoryItem(m_categoryModel->noCategoryText());
        return;
    }
    QString genreName=m_categoryModel->data(categoryIndex,
                                            Qt::DisplayRole).toString();
    //Set the category text.
    m_genreDisplay->setCategoryText(genreName);
    //Set the artwork.
    m_genreDisplay->setCategoryIcon(m_categoryModel->genreIcon(genreName));
}

void KNMusicLibraryGenreTab::onActionTabShow()
{
    //Do the original show request.
    KNMusicLibraryCategoryTab::onActionTabShow();
    //Ensure we have any artist item, and check whether the current index is vaild.
    checkCategorySelected();
    //Linked library not empty signal to category selected check.
    m_notEmptyCheck=connect(m_musicLibrary, &KNMusicLibraryModel::libraryNotEmpty,
                            this, &KNMusicLibraryGenreTab::checkCategorySelected);
}

void KNMusicLibraryGenreTab::onActionTabHide()
{
    //Do the original hide request.
    KNMusicLibraryCategoryTab::onActionTabHide();
    //Cut down the not empty check.
    disconnect(m_notEmptyCheck);
}

void KNMusicLibraryGenreTab::onActionRequireSearch()
{
    //Set focus.
    KNMusicGlobal::musicSearch()->setSearchFocus(m_genreList);
}

void KNMusicLibraryGenreTab::onActionShowInGenre()
{
    showInTab(KNMusicGlobal::soloMenu()->currentDetailInfo());
}

void KNMusicLibraryGenreTab::checkCategorySelected()
{
    if(proxyCategoryModel()->rowCount()>0 &&
            !m_genreList->currentIndex().isValid())
    {
        m_genreList->setCurrentIndex(proxyCategoryModel()->index(0,0));
    }
}

void KNMusicLibraryGenreTab::initialShowInAction()
{
    //Initial the show in action.
    m_showInGenre=new QAction(this);
    connect(m_showInGenre, SIGNAL(triggered()),
            this, SLOT(onActionShowInGenre()));
}

void KNMusicLibraryGenreTab::initialFindAction()
{
    //Initial the search action
    QAction *findAction=new QAction(m_dropProxy);
    findAction->setShortcut(QKeySequence(QKeySequence::Find));
    findAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(findAction, SIGNAL(triggered()), this, SLOT(onActionRequireSearch()));
    m_dropProxy->addAction(findAction);
}
