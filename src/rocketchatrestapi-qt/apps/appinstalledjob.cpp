/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinstalledjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

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
    addStartRestApiInfo(QByteArrayLiteral("AppInstalledJob: get app installed info starting"));
    return true;
}

void AppInstalledJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppInstalledJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appInstalledDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppInstalledJob: Problem when we tried to get app installed info : ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest AppInstalledJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::InstalledApps, QString(), RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_appinstalledjob.cpp"
