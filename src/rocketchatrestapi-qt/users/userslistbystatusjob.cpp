/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

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
    const QString loggedStr = mUsersListByStatusJobInfo.loggedInToString();
    if (!loggedStr.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("hasLoggedIn"), loggedStr);
    }
    const QString statusStr = mUsersListByStatusJobInfo.statusToString();
    if (!statusStr.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("status"), statusStr);
    }
    const QString typeStr = mUsersListByStatusJobInfo.typeToString();
    if (!typeStr.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("type"), typeStr);
    }
    if (!mUsersListByStatusJobInfo.roles.isEmpty()) {
        for (const auto &str : std::as_const(mUsersListByStatusJobInfo.roles)) {
            queryUrl.addQueryItem(QStringLiteral("roles[]"), str);
        }
    }
    if (!mUsersListByStatusJobInfo.searchName.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("searchTerm"), mUsersListByStatusJobInfo.searchName);
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool UsersListByStatusJob::hasQueryParameterSupport() const
{
    return true;
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
    case Status::Deactivated:
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

QString UsersListByStatusJob::UsersListByStatusJobInfo::loggedInToString() const
{
    switch (hasLoggedIn) {
    case LoggedStatus::Unknown:
        break;
    case LoggedStatus::Logged:
        return "true"_L1;
    case LoggedStatus::NotLogged:
        return "false"_L1;
    }
    return {};
}

QDebug operator<<(QDebug d, const RocketChatRestApi::UsersListByStatusJob::UsersListByStatusJobInfo &t)
{
    d << "status " << t.status;
    d << "type " << t.type;
    d << "hasLoggedIn " << t.hasLoggedIn;
    d << "roles " << t.roles;
    d << "searchName " << t.searchName;
    return d;
}

#include "moc_userslistbystatusjob.cpp"
