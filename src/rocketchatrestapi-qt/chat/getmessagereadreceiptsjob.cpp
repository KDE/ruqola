/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getmessagereadreceiptsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GetMessageReadReceiptsJob::GetMessageReadReceiptsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetMessageReadReceiptsJob::~GetMessageReadReceiptsJob() = default;

bool GetMessageReadReceiptsJob::requireHttpAuthentication() const
{
    return true;
}

bool GetMessageReadReceiptsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start get message read receipts job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("GetMessageReadReceiptsJob: get message read receipts starting"_ba);
    return true;
}

void GetMessageReadReceiptsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetMessageReadReceiptsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT getMessageReadReceiptsDone(*replyObject);
    }
}

QByteArray GetMessageReadReceiptsJob::messageId() const
{
    return mMessageId;
}

void GetMessageReadReceiptsJob::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest GetMessageReadReceiptsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatMessageReadReceipts);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"messageId"_s, QLatin1StringView(mMessageId));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool GetMessageReadReceiptsJob::canStart() const
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

#include "moc_getmessagereadreceiptsjob.cpp"
