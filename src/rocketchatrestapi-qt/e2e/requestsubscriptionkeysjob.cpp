/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "requestsubscriptionkeysjob.h"

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RequestSubscriptionKeysJob::RequestSubscriptionKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RequestSubscriptionKeysJob::~RequestSubscriptionKeysJob() = default;

bool RequestSubscriptionKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RequestSubscriptionKeysJob::start");
    submitPostRequest(json());
    return true;
}

void RequestSubscriptionKeysJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RequestSubscriptionKeysJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT requestSubscriptionKeysDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RequestSubscriptionKeysJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RequestSubscriptionKeysJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest RequestSubscriptionKeysJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2ERequestSubscriptionKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RequestSubscriptionKeysJob::json() const
{
    // The endpoint takes no argument: the server looks up our subscriptions without a key.
    const QJsonDocument postData = QJsonDocument(QJsonObject{});
    return postData;
}

#include "moc_requestsubscriptionkeysjob.cpp"
