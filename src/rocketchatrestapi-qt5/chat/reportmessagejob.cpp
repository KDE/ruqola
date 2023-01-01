/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ReportMessageJob::ReportMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ReportMessageJob::~ReportMessageJob() = default;

bool ReportMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ReportMessageJob::start");
    submitPostRequest(json());

    return true;
}

void ReportMessageJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    auto reply = mReply;
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("ReportMessageJob: success: ") + data);
        Q_EMIT reportMessageDone();
        reply->deleteLater();
    }
    deleteLater();
}

QString ReportMessageJob::reportMessage() const
{
    return mReportMessage;
}

void ReportMessageJob::setReportMessage(const QString &reportMessage)
{
    mReportMessage = reportMessage;
}

bool ReportMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool ReportMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReportMessageJob: mMessageId is empty";
        return false;
    }
    if (mReportMessage.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReportMessageJob: mReportMessage is empty";
        return false;
    }
    return true;
}

QNetworkRequest ReportMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatReportMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString ReportMessageJob::messageId() const
{
    return mMessageId;
}

void ReportMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QJsonDocument ReportMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("messageId")] = mMessageId;
    jsonObj[QLatin1String("description")] = mReportMessage;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
