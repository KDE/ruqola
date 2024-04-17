/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelannouncementjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ChangeChannelAnnouncementJob::ChangeChannelAnnouncementJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelAnnouncementJob::~ChangeChannelAnnouncementJob() = default;

bool ChangeChannelAnnouncementJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelAnnouncementJob::start");
    submitPostRequest(json());
    return true;
}

void ChangeChannelAnnouncementJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("Change announcement success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeAnnouncementDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("Problem when we tried to change announcement: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeChannelAnnouncementJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelAnnouncementJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelAnnouncementJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelAnnouncementJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = roomId();
    jsonObj["announcement"_L1] = announcement();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelAnnouncementJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelAnnouncementJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeChannelAnnouncementJob::announcement() const
{
    return mAnnouncement;
}

void ChangeChannelAnnouncementJob::setAnnouncement(const QString &t)
{
    mAnnouncement = t;
}

QNetworkRequest ChangeChannelAnnouncementJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changechannelannouncementjob.cpp"
