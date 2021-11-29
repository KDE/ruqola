/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelreadonlyjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeChannelReadonlyJob::ChangeChannelReadonlyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelReadonlyJob::~ChangeChannelReadonlyJob() = default;

bool ChangeChannelReadonlyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelReadonlyJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelReadonlyJob::slotChangeReadonlyFinished);
    return true;
}

void ChangeChannelReadonlyJob::slotChangeReadonlyFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("Change read only success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeReadonlyDone();
        } else {
            addLoggerInfo(QByteArrayLiteral("Problem when we tried to change read only status: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool ChangeChannelReadonlyJob::readOnly() const
{
    return mReadOnly;
}

void ChangeChannelReadonlyJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool ChangeChannelReadonlyJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelReadonlyJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelReadonlyJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelReadonlyJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("readOnly")] = readOnly();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelReadonlyJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelReadonlyJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeChannelReadonlyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetReadOnly);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
