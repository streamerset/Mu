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
#include "knmusicalbumtitle.h"

KNMusicAlbumTitle::KNMusicAlbumTitle(QWidget *parent) :
    QLabel(parent)
{
    //Change the focus policy.
    setFocusPolicy(Qt::StrongFocus);
}

void KNMusicAlbumTitle::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    checkMouseFlag();
}

void KNMusicAlbumTitle::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    //Release the flag.
    m_mouseIn=false;
    //Ask to hide the album art.
    emit requireHideAlbumArt();
}

void KNMusicAlbumTitle::mouseMoveEvent(QMouseEvent *event)
{
    QLabel::mouseMoveEvent(event);
    checkMouseFlag();
}

void KNMusicAlbumTitle::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    checkMouseFlag();
}
