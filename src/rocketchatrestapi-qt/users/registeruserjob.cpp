/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RegisterUserJob::RegisterUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RegisterUserJob::~RegisterUserJob() = default;

bool RegisterUserJob::canStart() const
{
    if (!mRegisterUserInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mRegisterUserInfo is empty"; // TODO improve it.
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool RegisterUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RegisterUserJob::start");
    submitPostRequest(json());
    return true;
}

void RegisterUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RegisterUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT registerUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RegisterUserJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

RegisterUserJob::RegisterUserInfo RegisterUserJob::registerUserInfo() const
{
    return mRegisterUserInfo;
}

void RegisterUserJob::setRegisterUserInfo(const RegisterUserInfo &registerUserInfo)
{
    mRegisterUserInfo = registerUserInfo;
}

QString RegisterUserJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "error-invalid-email"_L1) {
        const QString email = detail.value("email"_L1).toString();
        return i18n("Invalid Email \'%1\'.", email);
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

QNetworkRequest RegisterUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRegister);
    QNetworkRequest request(url);
    addRequestAttribute(request);
    return request;
}

bool RegisterUserJob::requireHttpAuthentication() const
{
    return false;
}

QJsonDocument RegisterUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["username"_L1] = mRegisterUserInfo.username;
    jsonObj["email"_L1] = mRegisterUserInfo.email;
    jsonObj["name"_L1] = mRegisterUserInfo.name;
    jsonObj["pass"_L1] = mRegisterUserInfo.password; // TODO ??
    if (!mRegisterUserInfo.reason.isEmpty()) {
        jsonObj["reason"_L1] = mRegisterUserInfo.reason;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RegisterUserJob::RegisterUserInfo::isValid() const
{
    return !username.trimmed().isEmpty() && !name.trimmed().isEmpty() && !email.trimmed().isEmpty() && !password.trimmed().isEmpty();
}

#include "moc_registeruserjob.cpp"
