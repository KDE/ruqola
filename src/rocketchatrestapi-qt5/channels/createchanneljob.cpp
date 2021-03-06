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

#include "createchanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateChannelJob::CreateChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateChannelJob::~CreateChannelJob()
{
}

bool CreateChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CreateChannelJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &CreateChannelJob::slotCreateChannelFinished);
    return true;
}

void CreateChannelJob::slotCreateChannelFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("CreateChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            qDebug() << " replyObject : " << replyObject;
            Q_EMIT createChannelDone(replyObject);
            if (!mCreateChannelInfo.password.isEmpty()) {
                const QJsonObject channelObj = replyObject[QStringLiteral("channel")].toObject();
                const QString channelId = channelObj[QStringLiteral("_id")].toString();
                if (channelId.isEmpty()) {
                    emitFailedMessage(replyObject, reply);
                    addLoggerWarning(QByteArrayLiteral("CreateChannelJob Impossible to extract channel id: ") + replyJson.toJson(QJsonDocument::Indented));
                } else {
                    Q_EMIT addJoinCodeToChannel(channelId, mCreateChannelInfo.password);
                }
            }
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("CreateChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CreateRoomInfo CreateChannelJob::createChannelInfo() const
{
    return mCreateChannelInfo;
}

void CreateChannelJob::setCreateChannelInfo(const CreateRoomInfo &createChannelInfo)
{
    mCreateChannelInfo = createChannelInfo;
}

QString CreateChannelJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-duplicate-channel-name")) {
        return i18n("A channel with name '%1' exists", detail.value(QStringLiteral("channel_name")).toString());
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

bool CreateChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateChannelJob::canStart() const
{
    if (!mCreateChannelInfo.canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateChannelJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateChannelJob::json() const
{
    return mCreateChannelInfo.json();
}

QNetworkRequest CreateChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
