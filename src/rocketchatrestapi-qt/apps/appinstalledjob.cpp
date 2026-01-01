/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinstalledjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppInstalledJob::AppInstalledJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppInstalledJob::~AppInstalledJob() = default;

bool AppInstalledJob::requireHttpAuthentication() const
{
    return true;
}

bool AppInstalledJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app installed job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("AppInstalledJob: get app installed info starting"_ba);
    return true;
}

void AppInstalledJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    Q_UNUSED(replyErrorString)
    // qDebug() << " replyJson " << replyJson;
    const QJsonArray replyArray = replyJson.object().value("apps"_L1).toArray();
    Q_EMIT appInstalledDone(replyArray);
#if 0
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AppInstalledJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appInstalledDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AppInstalledJob: Problem when we tried to get app installed info : "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
#endif
}

QNetworkRequest AppInstalledJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::InstalledApps, RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_appinstalledjob.cpp"
