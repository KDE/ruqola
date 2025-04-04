/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "stdoutqueuejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
StdoutQueueJob::StdoutQueueJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

StdoutQueueJob::~StdoutQueueJob() = default;

bool StdoutQueueJob::requireHttpAuthentication() const
{
    return true;
}

bool StdoutQueueJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start StdoutQueueJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("StdoutQueueJob: Ask for stdout queue"_ba);
    return true;
}

void StdoutQueueJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("StdoutQueueJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT stdoutQueueDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("StdoutQueueJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest StdoutQueueJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::StdoutQueue);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_stdoutqueuejob.cpp"
