/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslogoutotherclientsjob.h"

#include "restapimethod.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
        addLoggerInfo("UsersLogoutOtherClientsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersLogoutOtherClientsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersLogoutOtherClientsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool UsersLogoutOtherClientsJob::requireHttpAuthentication() const
{
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
