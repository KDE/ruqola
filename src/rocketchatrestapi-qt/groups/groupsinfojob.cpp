/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinfojob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("groupInfoDone success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelInfoDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("groupInfoDone problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GroupsInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsInfoJob: RoomId is empty";
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
    queryUrl.addQueryItem(u"roomId"_s, mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_groupsinfojob.cpp"
