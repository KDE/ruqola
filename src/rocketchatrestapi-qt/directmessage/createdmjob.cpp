/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CreateDmJob::CreateDmJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateDmJob::~CreateDmJob() = default;

bool CreateDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CreateDmJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void CreateDmJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("CreateDmJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT createDmDone(*replyObject);
    }
}

QStringList CreateDmJob::userNames() const
{
    return mUserNames;
}

void CreateDmJob::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

bool CreateDmJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateDmJob::canStart() const
{
    if (mUserNames.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateDmJob: usernames is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateDmJob::json() const
{
    QJsonObject jsonObj;
    if (mUserNames.count() == 1) {
        jsonObj["username"_L1] = mUserNames.at(0);
    } else {
        jsonObj["usernames"_L1] = mUserNames.join(u',');
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest CreateDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_createdmjob.cpp"
