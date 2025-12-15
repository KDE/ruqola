/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupaddownerjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
GroupAddOwnerJob::GroupAddOwnerJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupAddOwnerJob::~GroupAddOwnerJob() = default;

bool GroupAddOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupAddOwnerJob::start");
    submitPostRequest(json());

    return true;
}

void GroupAddOwnerJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupAddOwnerJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addOwnerDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupAddOwnerJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupAddOwnerJob::addownerUserId() const
{
    return mAddownerUserId;
}

void GroupAddOwnerJob::setAddownerUserId(const QString &addownerUserId)
{
    mAddownerUserId = addownerUserId;
}

bool GroupAddOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupAddOwnerJob::canStart() const
{
    if (mAddownerUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddOwnerJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddOwnerJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupAddOwnerJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = addownerUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupAddOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsAddOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupaddownerjob.cpp"
