/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listoauthappsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ListOauthAppsJob::ListOauthAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ListOauthAppsJob::~ListOauthAppsJob() = default;

bool ListOauthAppsJob::requireHttpAuthentication() const
{
    return false;
}

bool ListOauthAppsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListOauthAppsJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ListOauthAppsJob: Ask settings oauth"));
    return true;
}

void ListOauthAppsJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ListOauthAppsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT settingsOauthDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ListOauthAppsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest ListOauthAppsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
