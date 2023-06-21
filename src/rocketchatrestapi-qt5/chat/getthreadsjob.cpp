/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getthreadsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetThreadsJob::GetThreadsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetThreadsJob::~GetThreadsJob() = default;

bool GetThreadsJob::requireHttpAuthentication() const
{
    return true;
}

bool GetThreadsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetThreadsJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetThreadsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetThreadsJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetThreadsJob: Ask All threads in room"));
    return true;
}

void GetThreadsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetThreadsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getThreadsDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetThreadsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

GetThreadsJob::TheadSearchType GetThreadsJob::searchType() const
{
    return mSearchType;
}

void GetThreadsJob::setSearchType(TheadSearchType newSearchType)
{
    mSearchType = newSearchType;
}

QString GetThreadsJob::roomId() const
{
    return mRoomId;
}

void GetThreadsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetThreadsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetThreadsList);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("rid"), mRoomId);
    switch (mSearchType) {
    case TheadSearchType::All:
        queryUrl.addQueryItem(QStringLiteral("type"), QStringLiteral("all"));
        break;
    case TheadSearchType::Following:
        queryUrl.addQueryItem(QStringLiteral("type"), QStringLiteral("following"));
        break;
    case TheadSearchType::Unread:
        queryUrl.addQueryItem(QStringLiteral("type"), QStringLiteral("unread"));
        break;
    }

    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetThreadsJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_getthreadsjob.cpp"
