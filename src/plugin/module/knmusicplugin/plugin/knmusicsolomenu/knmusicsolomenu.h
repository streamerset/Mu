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
#ifndef KNMUSICSOLOMENU_H
#define KNMUSICSOLOMENU_H

#include "knmusicsolomenubase.h"

class QLineEdit;
class KNMessageBox;
class KNMusicSoloMenu : public KNMusicSoloMenuBase
{
    Q_OBJECT
public:
    explicit KNMusicSoloMenu(QWidget *parent = 0);
    ~KNMusicSoloMenu();
    void setProxyModel(KNMusicProxyModel *model);
    void setCurrentIndex(const QModelIndex &itemIndex);
    KNMusicDetailInfo currentDetailInfo();

signals:

public slots:
    void retranslate();
    void addMusicActions(QList<QAction *> actions);

private slots:
    void onActionOpenCurrent();
    void onActionShowInGraphicsShell();
    void onActionCopyFilePath();
    void onActionCopyItemText();
    void onActionSearchItemText();
    void onActionDownloadLyrics();
    void onActionShowDetail();
    void onActionRenameArtistHyphonName();
    void onActionRename();

private:
    enum SoloMenuActions
    {
        PlayCurrent,
        Open,
        GetInfo,
        ShowInGraphicShell,
        SearchItemText,
        DownloadLyrics,
        CopyFilePath,
        CopyItemText,
        Rename,
        RenameToArtistHyphonName,
        Delete,
        SoloMenuActionCount
    };
    void createActions();
    void initialRenameDialog();
    inline QString generatePreferFileName(const QModelIndex &itemIndex);
    KNMessageBox *m_renameDialog;
    QLineEdit *m_nameEdit;
    QString m_actionTitles[SoloMenuActionCount],
            m_itemText, m_filePath, m_preferFileName;
    QAction *m_actions[SoloMenuActionCount],
            *m_customSeperator;
    QModelIndex m_currentIndex;
    KNMusicProxyModel *m_proxyModel;

    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICSOLOMENU_H
