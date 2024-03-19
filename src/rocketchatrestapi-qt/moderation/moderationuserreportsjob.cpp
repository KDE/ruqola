/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationuserreportsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ModerationUserReportsJob::ModerationUserReportsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationUserReportsJob::~ModerationUserReportsJob() = default;

bool ModerationUserReportsJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationUserReportsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get get user reported message job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ModerationReportJob: get get user reported message starting"));
    return true;
}

void ModerationUserReportsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationUserReportedMessagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportJob: Problem when we tried to get user reported messages : ")
                         + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ModerationUserReportsJob::reportedMessageFromUserId() const
{
    return mReportedMessageFromUserId;
}

void ModerationUserReportsJob::setReportedMessageFromUserId(const QString &newReportedMessageFromUserId)
{
    mReportedMessageFromUserId = newReportedMessageFromUserId;
}

QNetworkRequest ModerationUserReportsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationUserReports);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("userId"), mReportedMessageFromUserId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool ModerationUserReportsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mReportedMessageFromUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

#include "moc_moderationuserreportsjob.cpp"
