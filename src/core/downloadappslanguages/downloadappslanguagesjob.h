/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef DOWNLOADAPPSLANGUAGESJOB_H
#define DOWNLOADAPPSLANGUAGESJOB_H

#include <QObject>
#include "libruqolacore_export.h"
class QNetworkAccessManager;
class QNetworkReply;
class LIBRUQOLACORE_EXPORT DownloadAppsLanguagesJob : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesJob(QObject *parent = nullptr);
    ~DownloadAppsLanguagesJob();

    Q_REQUIRED_RESULT bool canStart() const;
    void start();

    Q_REQUIRED_RESULT QString serverUrl() const;
    void setServerUrl(const QString &serverUrl);

Q_SIGNALS:
    void fileDownloaded(const QByteArray &data);

private:
    void slotFileDownloaded(QNetworkReply *reply);
    QString mServerUrl;
    QNetworkAccessManager *const mNetworkAccessManager;
};

#endif // DOWNLOADAPPSLANGUAGESJOB_H
