/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICLYRICSDOWNLOADDIALOGBASE_H
#define KNMUSICLYRICSDOWNLOADDIALOGBASE_H

#include "knmusicglobal.h"

#include "knmessagebox.h"

using namespace KNMusic;

class KNMusicBackend;
class KNMusicLyricsDownloadDialogBase : public KNMessageBox
{
    Q_OBJECT
public:
    KNMusicLyricsDownloadDialogBase(QWidget *parent = 0):KNMessageBox(parent){}
    virtual void setDetailInfo(const KNMusicDetailInfo &detailInfo)=0;
    virtual void setBackend(KNMusicBackend *backend)=0;

signals:
    void requireSearchLyrics(KNMusicDetailInfo detailInfo,
                             QStandardItemModel *lyricsModel);

public slots:
    virtual void onActionSearchComplete()=0;
};

#endif // KNMUSICLYRICSDOWNLOADDIALOGBASE_H
