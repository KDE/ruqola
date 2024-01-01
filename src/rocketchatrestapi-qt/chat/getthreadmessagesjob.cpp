/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

GetThreadMessagesJob::~GetThreadMessagesJob() = default;

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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetThreadMessagesJob: Ask threads messages"));
    return true;
}

void GetThreadMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetThreadMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getThreadMessagesDone(replyObject, mThreadMessageId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetThreadMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

QString GetThreadMessagesJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-invalid-message")) {
        return i18n("Invalid message: \'%1\'", mThreadMessageId);
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_getthreadmessagesjob.cpp"
