/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "getavatarjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetAvatarJob::GetAvatarJob(QObject *parent)
    : UserBaseJob(parent)
{
}

GetAvatarJob::~GetAvatarJob()
{
}

bool GetAvatarJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetAvatarJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetAvatarJob::slotGetAvatar);
    addStartRestApiInfo("GetAvatarJob ask for avatarUserId: " + mUserInfo.userIdentifier.toLatin1());
    addStartRestApiInfo("GetAvatarJob::start");
    return true;
}

void GetAvatarJob::slotGetAvatar()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QString userId = mUserInfo.userIdentifier;
        if (!reply->error()) {
            const QUrl url = reply->url();
            if (url.isValid() && !url.scheme().isEmpty()) {
                addLoggerInfo(QByteArrayLiteral("GetAvatarJob success: ") + userId.toUtf8());
                Q_EMIT avatar(mUserInfo, url);
            } else {
                qCWarning(ROCKETCHATQTRESTAPI_LOG) << "expected a URL, got something else:";
            }
        } else {
            addLoggerWarning(QByteArrayLiteral("GetAvatarJob error: ") + userId.toUtf8());
            Q_EMIT avatar(mUserInfo, QUrl());
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetAvatarJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar);
    addQueryUrl(url);
    QNetworkRequest request(url);
    return request;
}

bool GetAvatarJob::requireHttpAuthentication() const
{
    return false;
}

QString GetAvatarJob::jobName() const
{
    return i18n("Get Avatar");
}
