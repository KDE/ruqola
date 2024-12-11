/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppInfoJob::AppInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppInfoJob::~AppInfoJob() = default;

bool AppInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool AppInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app count job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("AppInfoJob: get app count info starting"));
    return true;
}

void AppInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appCountDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppInfoJob: Problem when we tried to get app count info : ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

AppInfoJob::AppInfoType AppInfoJob::appInfoType() const
{
    return mAppInfoType;
}

void AppInfoJob::setAppInfoType(AppInfoType newAppInfoType)
{
    mAppInfoType = newAppInfoType;
}

QByteArray AppInfoJob::appsId() const
{
    return mAppsId;
}

void AppInfoJob::setAppsId(const QByteArray &newAppsId)
{
    mAppsId = newAppsId;
}

QNetworkRequest AppInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CountApps, RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_appinfojob.cpp"
