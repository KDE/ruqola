/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "changechanneldescriptionjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeChannelDescriptionJob::ChangeChannelDescriptionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelDescriptionJob::~ChangeChannelDescriptionJob() = default;

bool ChangeChannelDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelDescriptionJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelDescriptionJob::slotChangeDescriptionFinished);
    return true;
}

void ChangeChannelDescriptionJob::slotChangeDescriptionFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Change description success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeDescriptionDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("Problem when we tried to change description: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChangeChannelDescriptionJob::description() const
{
    return mDescription;
}

void ChangeChannelDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeChannelDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelDescriptionJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelDescriptionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("description")] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelDescriptionJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelDescriptionJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeChannelDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
