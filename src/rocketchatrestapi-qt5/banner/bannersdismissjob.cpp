/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannersdismissjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
BannersDismissJob::BannersDismissJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

BannersDismissJob::~BannersDismissJob() = default;

bool BannersDismissJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("BannersDismissJob::start");
    submitPostRequest(json());

    return true;
}

void BannersDismissJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("BannersDismissJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT dimissBannerDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("BannersDismissJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &BannersDismissJob::bannerId() const
{
    return mBannerId;
}

void BannersDismissJob::setBannerId(const QString &newBannerId)
{
    mBannerId = newBannerId;
}

bool BannersDismissJob::requireHttpAuthentication() const
{
    return true;
}

bool BannersDismissJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mBannerId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "BannersDismissJob: mBannerId is empty";
        return false;
    }
    return true;
}

QNetworkRequest BannersDismissJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::BannersDismiss);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument BannersDismissJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("bannerId")] = mBannerId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_bannersdismissjob.cpp"
