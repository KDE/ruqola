/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "getthreadmessagesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetThreadMessagesJob::GetThreadMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetThreadMessagesJob::~GetThreadMessagesJob()
{
}

bool GetThreadMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetThreadMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mThreadMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetThreadMessagesJob: mThreadMessageId is empty";
        return false;
    }
    return true;
}

bool GetThreadMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetThreadMessagesJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetThreadMessagesJob::slotGetThreadMessagesFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetThreadMessagesJob: Ask threads messages"));
    return true;
}

void GetThreadMessagesJob::slotGetThreadMessagesFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetThreadMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getThreadMessagesDone(replyObject, mThreadMessageId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetThreadMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetThreadMessagesJob::threadMessageId() const
{
    return mThreadMessageId;
}

void GetThreadMessagesJob::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QNetworkRequest GetThreadMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("tmid"), mThreadMessageId);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    addAuthRawHeader(request);
    return request;
}

QString GetThreadMessagesJob::errorMessage(const QString &str)
{
    if (str == QLatin1String("error-invalid-message")) {
        return i18n("Invalid message: \'%1\'", mThreadMessageId);
    }
    return RestApiAbstractJob::errorMessage(str);
}
