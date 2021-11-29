/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "stdoutqueuejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &StdoutQueueJob::slotStdoutQueueFinished);
    addStartRestApiInfo(QByteArrayLiteral("StdoutQueueJob: Ask for server statistics"));
    return true;
}

void StdoutQueueJob::slotStdoutQueueFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("StdoutQueueJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT stdoutQueueDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("StdoutQueueJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest StdoutQueueJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::StdoutQueue);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
