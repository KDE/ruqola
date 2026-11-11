/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersenabletotpjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersEnableTotpJob::UsersEnableTotpJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersEnableTotpJob::~UsersEnableTotpJob() = default;

bool UsersEnableTotpJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersEnableTotpJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void UsersEnableTotpJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("UsersEnableTotpJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT usersEnableTotpDone(*replyObject);
    }
}

bool UsersEnableTotpJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest UsersEnableTotpJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersEnableTotp);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersEnableTotpJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_usersenabletotpjob.cpp"
