/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

ReportMessageJob::~ReportMessageJob()
{
}

bool ReportMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ReportMessageJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ReportMessageJob::slotReportMessage);
    return true;
}

void ReportMessageJob::slotReportMessage()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
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
