/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "subscriptiongetonejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SubscriptionGetOneJob::SubscriptionGetOneJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SubscriptionGetOneJob::~SubscriptionGetOneJob() = default;

bool SubscriptionGetOneJob::requireHttpAuthentication() const
{
    return true;
}

bool SubscriptionGetOneJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SubscriptionGetOneJob: RoomId is empty";
        return false;
    }
    return true;
}

bool SubscriptionGetOneJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SubscriptionGetOneJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("SubscriptionGetOneJob: Ask if room name already exists"_ba);
    return true;
}

void SubscriptionGetOneJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SubscriptionGetOneJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomInfoDone(replyObject["subscription"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SubscriptionGetOneJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QByteArray &SubscriptionGetOneJob::roomId() const
{
    return mRoomId;
}

void SubscriptionGetOneJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest SubscriptionGetOneJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsGetOne);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QString::fromLatin1(mRoomId));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_subscriptiongetonejob.cpp"
