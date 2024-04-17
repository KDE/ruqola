/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadmessagesjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetThreadMessagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getThreadMessagesDone(replyObject, mThreadMessageId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetThreadMessagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetThreadMessagesJob::threadMessageId() const
{
    return mThreadMessageId;
}

void GetThreadMessagesJob::setThreadMessageId(const QByteArray &threadMessageId)
{
    mThreadMessageId = threadMessageId;
}

QNetworkRequest GetThreadMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("tmid"), QLatin1StringView(mThreadMessageId));
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

QString GetThreadMessagesJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-invalid-message"_L1) {
        return i18n("Invalid message: \'%1\'", QLatin1StringView(mThreadMessageId));
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

#include "moc_getthreadmessagesjob.cpp"
