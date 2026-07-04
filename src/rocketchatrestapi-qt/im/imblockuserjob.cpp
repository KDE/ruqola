/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imblockuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ImBlockUserJob::ImBlockUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ImBlockUserJob::~ImBlockUserJob() = default;

bool ImBlockUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ImBlockUserJob::start");
    submitPostRequest(json());

    return true;
}

void ImBlockUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ImBlockUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT userBlockDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ImBlockUserJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ImBlockUserJob::blockUser() const
{
    return mBlockUser;
}

void ImBlockUserJob::setBlockUser(bool newBlockUser)
{
    mBlockUser = newBlockUser;
}

QByteArray ImBlockUserJob::roomId() const
{
    return mRoomId;
}

void ImBlockUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool ImBlockUserJob::requireHttpAuthentication() const
{
    return true;
}

bool ImBlockUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ImBlockUserJob: mRoomId is empty.";
        return false;
    }
    return true;
}

QNetworkRequest ImBlockUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImBlockUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ImBlockUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QLatin1StringView(mRoomId);
    jsonObj["block"_L1] = mBlockUser;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_imblockuserjob.cpp"
