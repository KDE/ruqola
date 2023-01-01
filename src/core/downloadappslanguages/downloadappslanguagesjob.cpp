/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesjob.h"
#include "ruqola_debug.h"

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

DownloadAppsLanguagesJob::~DownloadAppsLanguagesJob() = default;

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
