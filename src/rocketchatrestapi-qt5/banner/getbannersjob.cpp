/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getbannersjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
GetBannersJob::GetBannersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetBannersJob::~GetBannersJob() = default;

bool GetBannersJob::requireHttpAuthentication() const
{
    return true;
}

bool GetBannersJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetBannersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetBannersJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("GetBannersJob: Ask about banners"));
    return true;
}

void GetBannersJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetBannersJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getBannersDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GetBannersJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GetBannersJob::hasQueryParameterSupport() const
{
    return false;
}

QNetworkRequest GetBannersJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Banners);

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("platform"), QStringLiteral("web"));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
