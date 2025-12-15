/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
    addStartRestApiInfo("AppInfoJob: get app count info starting"_ba);
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
    if (mAppInfoType != AppInfoType::Languages) {
        if (mAppsId.isEmpty()) {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mAppsId is empty";
            return false;
        }
    }
    return true;
}

void AppInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AppInfoJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AppInfoJob: Problem when we tried to get app info : "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    QString url;
    if (!mAppsId.isEmpty()) {
        url += QString::fromLatin1(mAppsId) + u'/';
    }

    switch (mAppInfoType) {
    case AppInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Unknown type";
        break;
    case AppInfoType::Versions:
        url += u"versions"_s;
        break;
    case AppInfoType::Logs:
        url += u"logs"_s;
        break;
    case AppInfoType::Apis:
        url += u"apis"_s;
        break;
    case AppInfoType::Languages:
        url += u"languages"_s;
        break;
    case AppInfoType::ScreenShots:
        url += u"screenshots"_s;
        break;
    case AppInfoType::Settings:
        url += u"settings"_s;
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
