/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "channeljoinjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <KLocalizedString>

using namespace RocketChatRestApi;
ChannelJoinJob::ChannelJoinJob(QObject *parent)
    : ChannelBaseJob(parent)
{
}

ChannelJoinJob::~ChannelJoinJob()
{
}

bool ChannelJoinJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelJoinJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChannelJoinJob::slotChannelJoinFinished);
    return true;
}

void ChannelJoinJob::slotChannelJoinFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelJoinJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setChannelJoinDone(channelInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelJoinJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
            //Invalid password
            const QString errorType = replyObject[QStringLiteral("errorType")].toString();
            if (errorType == QLatin1String("error-code-invalid")) {
                Q_EMIT missingChannelPassword(channelInfo());
            } else if (errorType == QLatin1String("error-room-archived")) {
                Q_EMIT openArchivedRoom(channelInfo());
            }
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChannelJoinJob::joinCode() const
{
    return mJoinCode;
}

void ChannelJoinJob::setJoinCode(const QString &joinCode)
{
    mJoinCode = joinCode;
}

bool ChannelJoinJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelJoinJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelJoinJob: RoomId and RoomName are empty";
        return false;
    }
    return true;
}

QJsonDocument ChannelJoinJob::json() const
{
    QJsonObject jsonObj;
    generateJSon(jsonObj);
    if (!mJoinCode.isEmpty()) {
        jsonObj[QLatin1String("joinCode")] = mJoinCode;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelJoinJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsJoin);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString RocketChatRestApi::ChannelJoinJob::errorMessage(const QString &str)
{
    if (str == QLatin1String("error-room-not-found")) {
        return i18n("The required \'%1\' param provided does not match any channel", channelInfo().channelInfoIdentifier);
    }
    return ChannelBaseJob::errorMessage(str);
}
