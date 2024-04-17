/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportsjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ModerationReportsJob::ModerationReportsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ModerationReportsJob::~ModerationReportsJob() = default;

bool ModerationReportsJob::requireHttpAuthentication() const
{
    return true;
}

bool ModerationReportsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get moderation reports job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("ModerationReportJob: get moderation reports starting"));
    return true;
}

void ModerationReportsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ModerationReportJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT moderationReportsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ModerationReportJob: Problem when we tried to get moderation reports message : ")
                         + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray ModerationReportsJob::messageId() const
{
    return mMessageId;
}

void ModerationReportsJob::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest ModerationReportsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ModerationReports);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("msgId"), QLatin1StringView(mMessageId));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool ModerationReportsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

#include "moc_moderationreportsjob.cpp"
