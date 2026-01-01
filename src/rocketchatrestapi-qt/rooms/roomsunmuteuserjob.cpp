/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsunmuteuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsUnmuteUserJob::RoomsUnmuteUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsUnmuteUserJob::~RoomsUnmuteUserJob() = default;

bool RoomsUnmuteUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsUnmuteUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsUnmuteUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsUnmuteUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsUnmuteUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsUnmuteUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomsUnmuteUserJob::userName() const
{
    return mUserName;
}

void RoomsUnmuteUserJob::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QByteArray RoomsUnmuteUserJob::roomId() const
{
    return mRoomId;
}

void RoomsUnmuteUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsUnmuteUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsUnmuteUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserName.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsUnmuteUserJob: mUserName or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsUnmuteUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QString::fromLatin1(mRoomId);
    jsonObj["username"_L1] = mUserName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsUnmuteUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUnmuteUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsunmuteuserjob.cpp"
