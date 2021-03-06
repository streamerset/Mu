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
#ifndef KNMUSICLYRICSDOWNLOADER_H
#define KNMUSICLYRICSDOWNLOADER_H

#include "knglobal.h"
#include "knmusicglobal.h"

#include <QDebug>

#include <QObject>

namespace KNMusicLyricsData
{
struct KNMusicLyricsDetails
{
    int titleSimilarity;
    int artistSimilarity;
    QString title;
    QString artist;
    QString lyricsData;
};
}

using namespace KNMusic;
using namespace KNMusicLyricsData;

class QTimer;
class QNetworkReply;
class QNetworkAccessManager;
class KNMusicLyricsDownloader : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloader(QObject *parent = 0);
    ~KNMusicLyricsDownloader();
    void setWorkingThread(QThread *thread);
    virtual QString downloaderName()=0;
    virtual void downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                QList<KNMusicLyricsDetails> &lyricsList)=0;

signals:

public slots:

protected:
    inline QString processKeywords(QString str)
    {
        //Clear some no used words. I don't know how these regexp works.
        //Magic, don't touch.
        str=str.toLower();
        str.replace('.', " ");
        str.replace('&', " ");
//        str.replace(QRegExp("\\'|·|\\$|\\&|–"), " ");
        //truncate all symbols
        str.replace(QRegExp("\\(.*?\\)|\\[.*?]|{.*?}|\\uff08.*?\\uff09"), " ");
        str.replace(QRegExp("[-/:-@[-`{-~]+"), " ");
        return str;
    }
    void get(const QString &url,
             QByteArray &responseData,
             const QVariant &cookie=QVariant(),
             const QString &referer=QString());
    void post(const QString &url,
              QByteArray &responseData,
              const QByteArray &parameter,
              const QVariant &cookie=QVariant(),
              const QString &referer=QString());
    inline void saveLyrics(const KNMusicDetailInfo &detailInfo,
                           const QString &content,
                           KNMusicLyricsDetails &lyricsDetails,
                           QList<KNMusicLyricsDetails> &lyricsList)
    {
        lyricsDetails.titleSimilarity=
                KNGlobal::similarity(lyricsDetails.title, detailInfo.textLists[Name]);
        lyricsDetails.artistSimilarity=
                KNGlobal::similarity(lyricsDetails.artist, detailInfo.textLists[Artist]);
        lyricsDetails.lyricsData=content;
        //Add to list.
        lyricsList.append(lyricsDetails);
    }

private:
    enum NetworkProcessType
    {
        Post,
        Get
    };
    inline void networkProcess(int type,
                               const QString &url,
                               QByteArray &responseData,
                               const QByteArray &parameter,
                               const QVariant &cookie,
                               const QString &referer);
    QNetworkAccessManager *m_networkManager;
    QTimer *m_timeout;
};

#endif // KNMUSICLYRICSDOWNLOADER_H
