/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadavartarurlmanager.h"

#include "ruqola_debug.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

DownloadAvartarUrlManager::DownloadAvartarUrlManager(QObject *parent)
    : QObject(parent)
    , mNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &DownloadAvartarUrlManager::slotFileDownloaded);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &DownloadAvartarUrlManager::slotSslErrors);
}

DownloadAvartarUrlManager::~DownloadAvartarUrlManager() = default;

void DownloadAvartarUrlManager::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &error)
{
    reply->ignoreSslErrors(error);
}

void DownloadAvartarUrlManager::slotFileDownloaded(QNetworkReply *reply)
{
    const QByteArray data = reply->readAll();
    reply->deleteLater();
    Q_EMIT avatarDownloaded(data, mAvatarUrl);
    deleteLater();
}

bool DownloadAvartarUrlManager::canStart() const
{
    return !mAvatarUrl.isEmpty();
}

void DownloadAvartarUrlManager::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << " FileName is empty";
        deleteLater();
        return;
    }
    const QUrl url(mAvatarUrl);
    QNetworkRequest request(url);
    mNetworkAccessManager->get(request);
}

QString DownloadAvartarUrlManager::avatarUrl() const
{
    return mAvatarUrl;
}

void DownloadAvartarUrlManager::setAvatarUrl(const QString &avatarUrl)
{
    mAvatarUrl = avatarUrl;
}

#include "moc_downloadavartarurlmanager.cpp"
