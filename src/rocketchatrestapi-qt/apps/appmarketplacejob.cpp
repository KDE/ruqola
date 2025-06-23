/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appmarketplacejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AppMarketPlaceJob::AppMarketPlaceJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AppMarketPlaceJob::~AppMarketPlaceJob() = default;

bool AppMarketPlaceJob::requireHttpAuthentication() const
{
    return true;
}

bool AppMarketPlaceJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get app marketplace job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("AppMarketPlaceJob: get app marketplace info starting"_ba);
    return true;
}

void AppMarketPlaceJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    Q_UNUSED(replyErrorString)
    const QJsonArray replyArray = replyJson.array();
    Q_EMIT appMarketPlaceDone(replyArray);
#if 0
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AppMarketPlaceJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appMarketPlaceDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AppMarketPlaceJob: Problem when we tried to get app marketplace info : "_ba
                         + replyJson.toJson(QJsonDocument::Indented));
    }
#endif
}

bool AppMarketPlaceJob::isAdminUser() const
{
    return mIsAdminUser;
}

void AppMarketPlaceJob::setIsAdminUser(bool newIsAdminUser)
{
    mIsAdminUser = newIsAdminUser;
}

QNetworkRequest AppMarketPlaceJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::MarketplaceApps, RestApiUtil::RestApiUrlExtensionType::Apps);

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"isAdminUser"_s, mIsAdminUser ? u"true"_s : QStringLiteral("false"));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_appmarketplacejob.cpp"
