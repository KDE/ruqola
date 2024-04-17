/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ignoreuserjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace RocketChatRestApi;
IgnoreUserJob::IgnoreUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

IgnoreUserJob::~IgnoreUserJob() = default;

bool IgnoreUserJob::requireHttpAuthentication() const
{
    return true;
}

bool IgnoreUserJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ignore user job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("IgnoreUserJob: ignore user starting"));
    return true;
}

void IgnoreUserJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("IgnoreUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT ignoreUserDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("Problem when we tried to ignore user message: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool IgnoreUserJob::ignore() const
{
    return mIgnore;
}

void IgnoreUserJob::setIgnore(bool ignore)
{
    mIgnore = ignore;
}

QByteArray IgnoreUserJob::ignoreUserId() const
{
    return mIgnoreUserId;
}

void IgnoreUserJob::setIgnoreUserId(const QByteArray &ignoreUserId)
{
    mIgnoreUserId = ignoreUserId;
}

QByteArray IgnoreUserJob::roomId() const
{
    return mRoomId;
}

void IgnoreUserJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest IgnoreUserJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatIgnoreUser);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("rid"), QLatin1StringView(mRoomId));
    queryUrl.addQueryItem(QStringLiteral("userId"), QLatin1StringView(mIgnoreUserId));
    queryUrl.addQueryItem(QStringLiteral("ignore"), mIgnore ? QStringLiteral("true") : QStringLiteral("false"));
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool IgnoreUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomId is empty";
        return false;
    }
    if (mIgnoreUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ignore user id is empty";
        return false;
    }
    return true;
}

#include "moc_ignoreuserjob.cpp"
