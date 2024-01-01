/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersUpdateJob::UsersUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateJob::~UsersUpdateJob() = default;

bool UsersUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateJob::start");
    submitPostRequest(json());
    return true;
}

void UsersUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

UpdateUserInfo UsersUpdateJob::updateInfo() const
{
    return mUpdateInfo;
}

void UsersUpdateJob::setUpdateInfo(const UpdateUserInfo &updateInfo)
{
    mUpdateInfo = updateInfo;
}

bool UsersUpdateJob::requireTwoFactorAuthentication() const
{
    return true;
}

QString UsersUpdateJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-email-send-failed")) {
        return i18n("Error trying to send email %1", mUpdateInfo.mEmail);
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

bool UsersUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateJob: mUpdateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersUpdateJob::json() const
{
    auto t = mUpdateInfo.json();
    return t;
}

#include "moc_usersupdatejob.cpp"
