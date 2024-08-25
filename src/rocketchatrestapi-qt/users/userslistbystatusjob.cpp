/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslistbystatusjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
UsersListByStatusJob::UsersListByStatusJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersListByStatusJob::~UsersListByStatusJob() = default;

bool UsersListByStatusJob::requireHttpAuthentication() const
{
    return true;
}

// TODO implement void initialUrlParameters(QUrlQuery &urlQuery) const
// https://<foo>/api/v1/users.listByStatus?count=25&offset=0&searchTerm=&sort=%7B%20%22name%22%3A%201%20%7D
bool UsersListByStatusJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start userspresence job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("UsersListByStatusJob: Ask info about me"));
    return true;
}

void UsersListByStatusJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersListByStatusJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersPresenceDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersListByStatusJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest UsersListByStatusJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersPresence);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool UsersListByStatusJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

#include "moc_userslistbystatusjob.cpp"
