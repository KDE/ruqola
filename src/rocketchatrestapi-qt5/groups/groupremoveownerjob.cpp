/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveownerjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

void GroupRemoveOwnerJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GroupRemoveOwnerJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT groupRemoveOwnerDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GroupRemoveOwnerJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        if (replyObject[QStringLiteral("errorType")].toString() == QLatin1String("error-remove-last-owner")) {
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
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveOwnerJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupRemoveOwnerJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupRemoveOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
