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
#ifndef KNMUSICPLAYLISTLISTVIEWEDITOR_H
#define KNMUSICPLAYLISTLISTVIEWEDITOR_H

#include <QStringList>

#include "knlinearsensewidget.h"

class KNAnimationMenu;
class KNOpacityAnimeButton;
class KNMusicPlaylistLoader;
class KNMusicPlaylistListView;
class KNMusicPlaylistListViewEditor : public KNLinearSenseWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListViewEditor(QWidget *parent = 0);
    void setPlaylistLoader(KNMusicPlaylistLoader *playlistLoader);
    void setPlaylistListView(KNMusicPlaylistListView *playlistListView);

signals:
    void requireAddPlaylist();
    void requireImportPlaylist(const QStringList &playlistList);
    void requireExportPlaylist(const QString &filePath,
                               const QModelIndex &index);
    void requireRemoveCurrentPlaylist();
    void requireCopyCurrentPlaylist();

public slots:
    void retranslate();
    void importPlaylists();
    void exportPlaylist();

private slots:
    void showAddMenu();
    void showConfigureMenu();

private:
    inline void initialMenu();
    void updateDialogFilter();
    enum AddMenuAction
    {
        AddPlaylist,
        ImportPlaylist,
        AddMenuActionsCount
    };
    enum ConfigureMenuAction
    {
        ExportPlaylist,
        CopyPlaylist,
        ConfigureMenuActionCount
    };
    QStringList m_playlistFilter;
    QAction *m_addActions[AddMenuActionsCount],
            *m_configureActions[ConfigureMenuActionCount];
    KNAnimationMenu *m_addMenu, *m_configureMenu;
    KNOpacityAnimeButton *m_add, *m_removeCurrent, *m_configure;
    KNMusicPlaylistLoader *m_playlistLoader;
    KNMusicPlaylistListView *m_playlistListView;
};

#endif // KNMUSICPLAYLISTLISTVIEWEDITOR_H
