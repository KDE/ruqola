/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listoauthappsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ListOauthAppsJob::ListOauthAppsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ListOauthAppsJob::~ListOauthAppsJob() = default;

bool ListOauthAppsJob::requireHttpAuthentication() const
{
    return true;
}

bool ListOauthAppsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ListOauthAppsJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("ListOauthAppsJob: Ask settings oauth"_ba);
    return true;
}

void ListOauthAppsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("ListOauthAppsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT listOauthDone(*replyObject);
    }
}

QNetworkRequest ListOauthAppsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::OauthAppsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_listoauthappsjob.cpp"
