/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersdisabletotpjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersDisableTotpJob::UsersDisableTotpJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersDisableTotpJob::~UsersDisableTotpJob() = default;

bool UsersDisableTotpJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersDisableTotpJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void UsersDisableTotpJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("UsersDisableTotpJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT usersDisableTotpDone(*replyObject);
    }
}

QString UsersDisableTotpJob::code() const
{
    return mCode;
}

void UsersDisableTotpJob::setCode(const QString &code)
{
    mCode = code;
}

bool UsersDisableTotpJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersDisableTotpJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCode.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersDisableTotpJob: mCode is empty";
        return false;
    }
    return true;
}

QNetworkRequest UsersDisableTotpJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersDisableTotp);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersDisableTotpJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["code"_L1] = mCode;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_usersdisabletotpjob.cpp"
