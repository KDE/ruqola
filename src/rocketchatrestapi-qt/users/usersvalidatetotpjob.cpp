/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersvalidatetotpjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersValidateTotpJob::UsersValidateTotpJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersValidateTotpJob::~UsersValidateTotpJob() = default;

bool UsersValidateTotpJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersValidateTotpJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void UsersValidateTotpJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("UsersValidateTotpJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT usersValidateTotpDone(*replyObject);
    }
}

QString UsersValidateTotpJob::code() const
{
    return mCode;
}

void UsersValidateTotpJob::setCode(const QString &code)
{
    mCode = code;
}

bool UsersValidateTotpJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersValidateTotpJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCode.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersValidateTotpJob: mCode is empty";
        return false;
    }
    return true;
}

QNetworkRequest UsersValidateTotpJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersValidateTotp);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersValidateTotpJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["code"_L1] = mCode;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_usersvalidatetotpjob.cpp"
