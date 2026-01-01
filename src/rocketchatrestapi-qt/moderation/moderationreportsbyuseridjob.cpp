/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsbyuseridjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
ModerationReportsByUserIdJob::ModerationReportsByUserIdJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportsByUserIdJob::~ModerationReportsByUserIdJob() = default;

bool ModerationReportsByUserIdJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportsByUserIdJob::hasQueryParameterSupport() const
{
    return true;
}

bool ModerationReportsByUserIdJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mModerationUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mModerationUserId is empty";
        return false;
    }
    return true;
}

bool ModerationReportsByUserIdJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ModerationReportsByUserIdJob";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("ModerationReportsByUserIdJob: Ask for moderation reportes by users"_ba);
    return true;
}

void ModerationReportsByUserIdJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ModerationReportsByUserIdJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportsByUserIdDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ModerationReportsByUserIdJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray ModerationReportsByUserIdJob::moderationUserId() const
{
    return mModerationUserId;
}

void ModerationReportsByUserIdJob::setModerationUserId(const QByteArray &newModerationUserId)
{
    mModerationUserId = newModerationUserId;
}

QNetworkRequest ModerationReportsByUserIdJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReportsByUserId);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"userId"_s, QString::fromLatin1(mModerationUserId));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_moderationreportsbyuseridjob.cpp"
