/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "downloadappslanguagesjob.h"
#include "ruqola_debug.h"
#include "utils.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <restapiutil.h>

DownloadAppsLanguagesJob::DownloadAppsLanguagesJob(QObject *parent)
    : QObject(parent)
    , mNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &DownloadAppsLanguagesJob::slotFileDownloaded);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &DownloadAppsLanguagesJob::slotSslErrors);
}

DownloadAppsLanguagesJob::~DownloadAppsLanguagesJob()
{
}

void DownloadAppsLanguagesJob::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    reply->ignoreSslErrors(error);
}

void DownloadAppsLanguagesJob::slotFileDownloaded(QNetworkReply *reply)
{
    const QByteArray data = reply->readAll();
    reply->deleteLater();
    Q_EMIT fileDownloaded(data);
    deleteLater();
}

bool DownloadAppsLanguagesJob::canStart() const
{
    return !mServerUrl.isEmpty();
}

void DownloadAppsLanguagesJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << " FileName is empty";
        deleteLater();
        return;
    }
    const QString urlStr = RestApiUtil::adaptUrl(mServerUrl) + QStringLiteral("/api/apps/languages");
    const QUrl url(urlStr);
    QNetworkRequest request(url);
    mNetworkAccessManager->get(request);
}

QString DownloadAppsLanguagesJob::serverUrl() const
{
    return mServerUrl;
}

void DownloadAppsLanguagesJob::setServerUrl(const QString &serverUrl)
{
    mServerUrl = serverUrl;
}
