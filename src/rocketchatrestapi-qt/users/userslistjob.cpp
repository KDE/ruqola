/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersListJob::UsersListJob(QObject *parent)
    : UserBaseJob(parent)
{
}

UsersListJob::~UsersListJob() = default;

bool UsersListJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersListJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("UsersListJob: ask users list"_ba);
    return true;
}

void UsersListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersListJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT userListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest UsersListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersList);
    QUrlQuery queryUrl;
    addQueryUrl(url);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool UsersListJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_userslistjob.cpp"
