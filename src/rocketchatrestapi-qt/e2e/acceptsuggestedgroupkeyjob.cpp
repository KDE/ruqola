/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "acceptsuggestedgroupkeyjob.h"
#include "rocketchatqtrestapi_debug.h"

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AcceptSuggestedGroupKeyJob::AcceptSuggestedGroupKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AcceptSuggestedGroupKeyJob::~AcceptSuggestedGroupKeyJob() = default;

bool AcceptSuggestedGroupKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("AcceptSuggestedGroupKeyJob::start");
    submitPostRequest(json());
    return true;
}

void AcceptSuggestedGroupKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AcceptSuggestedGroupKeyJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT acceptSuggestedGroupKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AcceptSuggestedGroupKeyJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString AcceptSuggestedGroupKeyJob::getRoomId() const
{
    return mRoomId;
}

void AcceptSuggestedGroupKeyJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

bool AcceptSuggestedGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest AcceptSuggestedGroupKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EAcceptSuggestedGroupKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument AcceptSuggestedGroupKeyJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["rid"_L1] = mRoomId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool AcceptSuggestedGroupKeyJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RejectSuggestedGroupKeyJob: roomId is empty";
        return false;
    }
    return true;
}
#include "moc_acceptsuggestedgroupkeyjob.cpp"
