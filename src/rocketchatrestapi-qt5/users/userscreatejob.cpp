/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userscreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersCreateJob::UsersCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersCreateJob::~UsersCreateJob() = default;

bool UsersCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersCreateJob::start");
    submitPostRequest(json());
    return true;
}

void UsersCreateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersCreateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersCreateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

CreateUpdateUserInfo UsersCreateJob::createInfo() const
{
    return mCreateInfo;
}

void UsersCreateJob::setCreateInfo(const CreateUpdateUserInfo &createInfo)
{
    mCreateInfo = createInfo;
}

bool UsersCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersCreateJob: mCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersCreateJob::json() const
{
    return mCreateInfo.json();
}

QString UsersCreateJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    // qDebug() << " str " << str << " details " << detail;
    if (str == QStringLiteral("error-field-unavailable")) {
        const QString field = detail.value(QLatin1String("field")).toString();
        return i18n("%1 is already in use.", field);
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}
