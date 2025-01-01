/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslogoutotherclientsjob.h"

#include "restapimethod.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersLogoutOtherClientsJob::UsersLogoutOtherClientsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersLogoutOtherClientsJob::~UsersLogoutOtherClientsJob() = default;

bool UsersLogoutOtherClientsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersLogoutOtherClientsJob::start");
    submitPostRequest(json());
    return true;
}

void UsersLogoutOtherClientsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersLogoutOtherClientsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersLogoutOtherClientsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersLogoutOtherClientsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool UsersLogoutOtherClientsJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersLogoutOtherClientsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest UsersLogoutOtherClientsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersLogoutOtherClients);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersLogoutOtherClientsJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_userslogoutotherclientsjob.cpp"
