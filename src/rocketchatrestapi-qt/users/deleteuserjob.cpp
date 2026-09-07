/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deleteuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
DeleteUserJob::DeleteUserJob(QObject *parent)
    : UserBaseJob(parent)
{
}

DeleteUserJob::~DeleteUserJob() = default;

bool DeleteUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteUserJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void DeleteUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("DeleteUserJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT deleteUserDone();
    }
}

bool DeleteUserJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteUserJob: identifier is empty";
        return false;
    }
    return true;
}

QNetworkRequest DeleteUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument DeleteUserJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_deleteuserjob.cpp"
