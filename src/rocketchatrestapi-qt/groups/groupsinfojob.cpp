/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GroupsInfoJob::GroupsInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GroupsInfoJob::~GroupsInfoJob() = default;

bool GroupsInfoJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupsInfoJob::start: ");
    submitGetRequest();

    return true;
}

void GroupsInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("groupInfoDone success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelInfoDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("groupInfoDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GroupsInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsInfoJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsInfoJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QString GroupsInfoJob::roomId() const
{
    return mRoomId;
}

void GroupsInfoJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GroupsInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsInfo);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_groupsinfojob.cpp"
