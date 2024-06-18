/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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
    addStartRestApiInfo(QByteArrayLiteral("AppMarketPlaceJob: get app marketplace info starting"));
    return true;
}

void AppMarketPlaceJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonArray replyArray = replyJson.array();
    Q_EMIT appMarketPlaceDone(replyArray);
#if 0
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("AppMarketPlaceJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT appMarketPlaceDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("AppMarketPlaceJob: Problem when we tried to get app marketplace info : ")
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
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::MarketplaceApps, QString(), RestApiUtil::RestApiUrlExtensionType::Apps);

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("isAdminUser"), mIsAdminUser ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_appmarketplacejob.cpp"
