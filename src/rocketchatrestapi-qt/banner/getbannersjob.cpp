/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getbannersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GetBannersJob::GetBannersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetBannersJob::~GetBannersJob() = default;

bool GetBannersJob::requireHttpAuthentication() const
{
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
    addStartRestApiInfo("GetBannersJob: Ask about banners"_ba);
    return true;
}

void GetBannersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetBannersJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getBannersDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetBannersJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(u"platform"_s, u"web"_s);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_getbannersjob.cpp"
