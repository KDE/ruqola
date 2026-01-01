/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userspresencejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersPresenceJob::UsersPresenceJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersPresenceJob::~UsersPresenceJob() = default;

bool UsersPresenceJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersPresenceJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start userspresence job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("UsersPresenceJob: Ask info about me"_ba);
    return true;
}

void UsersPresenceJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersPresenceJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersPresenceDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersPresenceJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest UsersPresenceJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersPresence);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_userspresencejob.cpp"
