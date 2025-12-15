/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveownerjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
GroupRemoveOwnerJob::GroupRemoveOwnerJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupRemoveOwnerJob::~GroupRemoveOwnerJob() = default;

bool GroupRemoveOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupRemoveOwnerJob::start");
    submitPostRequest(json());

    return true;
}

void GroupRemoveOwnerJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupRemoveOwnerJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT groupRemoveOwnerDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupRemoveOwnerJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        if (replyObject["errorType"_L1].toString() == "error-remove-last-owner"_L1) {
            Q_EMIT failed(i18n("This is the last owner. Please set a new owner before removing this one."));
        }
    }
}

QString GroupRemoveOwnerJob::removeUserId() const
{
    return mRemoveUserId;
}

void GroupRemoveOwnerJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool GroupRemoveOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupRemoveOwnerJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: RoomId is empty";
        return false;
    }
    return true;
}

QJsonDocument GroupRemoveOwnerJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupRemoveOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupremoveownerjob.cpp"
