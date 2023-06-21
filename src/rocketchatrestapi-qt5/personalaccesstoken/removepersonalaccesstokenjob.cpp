/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removepersonalaccesstokenjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
RemovePersonalAccessTokenJob::RemovePersonalAccessTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RemovePersonalAccessTokenJob::~RemovePersonalAccessTokenJob() = default;

bool RemovePersonalAccessTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RemovePersonalAccessTokenJob::start");
    submitPostRequest(json());

    return true;
}

void RemovePersonalAccessTokenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RemovePersonalAccessTokenJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeTokenDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RemovePersonalAccessTokenJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &RemovePersonalAccessTokenJob::tokenName() const
{
    return mTokenName;
}

void RemovePersonalAccessTokenJob::setTokenName(const QString &newTokenName)
{
    mTokenName = newTokenName;
}

bool RemovePersonalAccessTokenJob::requireHttpAuthentication() const
{
    return true;
}

bool RemovePersonalAccessTokenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTokenName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RemovePersonalAccessTokenJob: mTokenName is empty";
        return false;
    }
    return true;
}

QNetworkRequest RemovePersonalAccessTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRemovePersonalAccessToken);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RemovePersonalAccessTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("tokenName")] = mTokenName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RemovePersonalAccessTokenJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_removepersonalaccesstokenjob.cpp"
