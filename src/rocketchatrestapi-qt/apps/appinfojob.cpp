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

bool AppInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mAppInfoType == AppInfoType::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppInfoType undefined";
        return false;
    }
    if (mAppsId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppsId is empty";
        return false;
    }
    return true;
}

void AppInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appInfoDone(mAppInfoType, replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppInfoJob: Problem when we tried to get app info : ") + replyJson.toJson(QJsonDocument::Indented));
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

QString AppInfoJob::generateUrlExtension() const
{
    QString url = QString::fromLatin1(mAppsId) + QLatin1Char('/');

    switch (mAppInfoType) {
    case AppInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Unknown type";
        break;
    case AppInfoType::Versions:
        url += QStringLiteral("versions");
        break;
    case AppInfoType::Logs:
        url += QStringLiteral("logs");
        break;
    case AppInfoType::Apis:
        url += QStringLiteral("apis");
        break;
    case AppInfoType::ScreenShots:
        url += QStringLiteral("screenshots");
        break;
    }
    return url;
}

QNetworkRequest AppInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Empty, RestApiUtil::RestApiUrlExtensionType::Apps, generateUrlExtension());
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_appinfojob.cpp"
