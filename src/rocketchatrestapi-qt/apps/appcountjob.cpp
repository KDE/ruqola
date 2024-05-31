/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcountjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppCountJob::AppCountJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppCountJob::~AppCountJob() = default;

bool AppCountJob::requireHttpAuthentication() const
{
    return true;
}

bool AppCountJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app count job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("AppCountJob: get report info starting"));
    return true;
}

void AppCountJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppCountJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppCountJob: Problem when we tried to get app count info : ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest AppCountJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CountApps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_appcountjob.cpp"
