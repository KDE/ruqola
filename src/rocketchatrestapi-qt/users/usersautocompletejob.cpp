/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersautocompletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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
    addStartRestApiInfo("UsersAutocompleteJob: Ask info about me"_ba);
    return true;
}

void UsersAutocompleteJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersAutocompleteJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersAutocompleteDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersAutocompleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    // const QString val = u"{\"term\": \"%1\", \"exception\": \"%2\"}"_s.arg(mUsersCompleterInfo.pattern).arg(mUsersCompleterInfo.exception);
    const QString val = u"{\"term\": \"%1\"}"_s.arg(mUsersCompleterInfo.pattern);
    queryUrl.addQueryItem(u"selector"_s, val);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

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

#include "moc_usersautocompletejob.cpp"
