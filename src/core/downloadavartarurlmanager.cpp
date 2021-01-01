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

#include "downloadavartarurlmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ruqola_debug.h"

DownloadAvartarUrlManager::DownloadAvartarUrlManager(QObject *parent)
    : QObject(parent)
    , mNetworkAccessManager(new QNetworkAccessManager(this))
{
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &DownloadAvartarUrlManager::slotFileDownloaded);
    connect(mNetworkAccessManager, &QNetworkAccessManager::sslErrors, this, &DownloadAvartarUrlManager::slotSslErrors);
}

DownloadAvartarUrlManager::~DownloadAvartarUrlManager()
{
}

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
