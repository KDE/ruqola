/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "subscriptiongetalljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SubscriptionGetAllJob::SubscriptionGetAllJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SubscriptionGetAllJob::~SubscriptionGetAllJob() = default;

bool SubscriptionGetAllJob::requireHttpAuthentication() const
{
    return true;
}

bool SubscriptionGetAllJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SubscriptionGetAllJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("SubscriptionGetAllJob: Ask if room name already exists"_ba);
    return true;
}

void SubscriptionGetAllJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SubscriptionGetAllJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT subscriptionGetAllDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SubscriptionGetAllJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest SubscriptionGetAllJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsGetAll);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_subscriptiongetalljob.cpp"
