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
// https://<foo>/api/v1/users.listByStatus?count=1&hasLoggedIn=false&status=deactivated&type=user
bool UsersListByStatusJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersListByStatus job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("UsersListByStatusJob: ask list of user "));
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

UsersListByStatusJob::UsersListByStatusJobInfo UsersListByStatusJob::usersListByStatusJobInfo() const
{
    return mUsersListByStatusJobInfo;
}

void UsersListByStatusJob::setUsersListByStatusJobInfo(const UsersListByStatusJobInfo &newUsersListByStatusJobInfo)
{
    mUsersListByStatusJobInfo = newUsersListByStatusJobInfo;
}

QNetworkRequest UsersListByStatusJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersListByStatus);

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

bool UsersListByStatusJob::UsersListByStatusJobInfo::isValid() const
{
    return status != Status::Unknown && type != StatusType::Unknown;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo &t)
{
    d << "status " << t.status;
    d << "type " << t.type;
    d << "hasLoggedIn " << t.hasLoggedIn;
    d << "count " << t.count;
    return d;
}

#include "moc_userslistbystatusjob.cpp"
