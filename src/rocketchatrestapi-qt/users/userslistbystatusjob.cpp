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
#include <QUrlQuery>
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
        Q_EMIT usersListByStatusDone(replyObject);
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

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("hasLoggedIn"), mUsersListByStatusJobInfo.hasLoggedIn ? QStringLiteral("true") : QStringLiteral("false"));
    const QString statusStr = mUsersListByStatusJobInfo.statusToString();
    if (!statusStr.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("status"), statusStr);
    }
    const QString typeStr = mUsersListByStatusJobInfo.typeToString();
    if (!typeStr.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("type"), mUsersListByStatusJobInfo.typeToString());
    }
    if (!mUsersListByStatusJobInfo.roles.isEmpty()) {
        for (const auto &str : std::as_const(mUsersListByStatusJobInfo.roles)) {
            queryUrl.addQueryItem(QStringLiteral("roles[]"), str);
        }
    }
    queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mUsersListByStatusJobInfo.count));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

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
    if (!mUsersListByStatusJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mUsersListByStatusJobInfo is not valid";
        return false;
    }
    return true;
}

bool UsersListByStatusJob::UsersListByStatusJobInfo::isValid() const
{
    return true;
}

QString UsersListByStatusJob::UsersListByStatusJobInfo::statusToString() const
{
    switch (status) {
    case Status::Unknown:
        break;
    case Status::Desactivated:
        return "deactivated"_L1;
    case Status::Activated:
        return "active"_L1;
    }
    return {};
}

QString UsersListByStatusJob::UsersListByStatusJobInfo::typeToString() const
{
    switch (type) {
    case StatusType::Unknown:
        break;
    case StatusType::User:
        return "user"_L1;
    }
    return {};
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo &t)
{
    d << "status " << t.status;
    d << "type " << t.type;
    d << "hasLoggedIn " << t.hasLoggedIn;
    d << "count " << t.count;
    d << "roles " << t.roles;
    return d;
}

#include "moc_userslistbystatusjob.cpp"
