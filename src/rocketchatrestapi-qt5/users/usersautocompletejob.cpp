/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersautocompletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
UsersAutocompleteJob::UsersAutocompleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersAutocompleteJob::~UsersAutocompleteJob() = default;

bool UsersAutocompleteJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersAutocompleteJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersAutocompleteJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("UsersAutocompleteJob: Ask info about me"));
    return true;
}

void UsersAutocompleteJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UsersAutocompleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersAutocompleteDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("UsersAutocompleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

UsersAutocompleteJob::UsersAutocompleterInfo UsersAutocompleteJob::usersCompleterInfo() const
{
    return mUsersCompleterInfo;
}

void UsersAutocompleteJob::setUsersCompleterInfo(const UsersAutocompleterInfo &usersCompleterInfo)
{
    mUsersCompleterInfo = usersCompleterInfo;
}

QNetworkRequest UsersAutocompleteJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersAutocomplete);
    QUrlQuery queryUrl;
    // TODO add support for exception.
    // const QString val = QStringLiteral("{\"term\": \"%1\", \"exception\": \"%2\"}").arg(mUsersCompleterInfo.pattern).arg(mUsersCompleterInfo.exception);
    const QString val = QStringLiteral("{\"term\": \"%1\"}").arg(mUsersCompleterInfo.pattern);
    queryUrl.addQueryItem(QStringLiteral("selector"), val);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool UsersAutocompleteJob::canStart() const
{
    if (!mUsersCompleterInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersAutocompleteJob: selector is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool UsersAutocompleteJob::UsersAutocompleterInfo::isValid() const
{
    return !pattern.isEmpty();
}
