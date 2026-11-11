/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersregeneratetotpcodesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersRegenerateTotpCodesJob::UsersRegenerateTotpCodesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersRegenerateTotpCodesJob::~UsersRegenerateTotpCodesJob() = default;

bool UsersRegenerateTotpCodesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersRegenerateTotpCodesJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void UsersRegenerateTotpCodesJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("UsersRegenerateTotpCodesJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT usersRegenerateTotpCodesDone(*replyObject);
    }
}

QString UsersRegenerateTotpCodesJob::code() const
{
    return mCode;
}

void UsersRegenerateTotpCodesJob::setCode(const QString &code)
{
    mCode = code;
}

bool UsersRegenerateTotpCodesJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersRegenerateTotpCodesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCode.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersRegenerateTotpCodesJob: mCode is empty";
        return false;
    }
    return true;
}

QNetworkRequest UsersRegenerateTotpCodesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRegenerateTotpCodes);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersRegenerateTotpCodesJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["code"_L1] = mCode;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_usersregeneratetotpcodesjob.cpp"
