/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rejectsuggestedgroupkeyjob.h"
#include "rocketchatqtrestapi_debug.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
RejectSuggestedGroupKeyJob::RejectSuggestedGroupKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RejectSuggestedGroupKeyJob::~RejectSuggestedGroupKeyJob() = default;

bool RejectSuggestedGroupKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RejectSuggestedGroupKeyJob::start");
    submitPostRequest(json());
    return true;
}

void RejectSuggestedGroupKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RejectSuggestedGroupKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT rejectSuggestedGroupKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RejectSuggestedGroupKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RejectSuggestedGroupKeyJob::getRoomId() const
{
    return mRoomId;
}

void RejectSuggestedGroupKeyJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

bool RejectSuggestedGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest RejectSuggestedGroupKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2ERejectSuggestedGroupKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RejectSuggestedGroupKeyJob::json() const
{
    QJsonObject jsonObj;
    // TODO
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RejectSuggestedGroupKeyJob::canStart() const
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

#include "moc_rejectsuggestedgroupkeyjob.cpp"
