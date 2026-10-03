/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcountjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
    addStartRestApiInfo("AppCountJob: get app count info starting"_ba);
    return true;
}

void AppCountJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("AppCountJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT appCountDone(*replyObject);
    }
}

QNetworkRequest AppCountJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CountApps, RestApiUtil::RestApiUrlExtensionType::Apps);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_appcountjob.cpp"
