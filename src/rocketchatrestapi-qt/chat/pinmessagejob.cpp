/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pinmessagejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
PinMessageJob::PinMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PinMessageJob::~PinMessageJob() = default;

bool PinMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("PinMessageJob::start");
    submitPostRequest(json());

    return true;
}

void PinMessageJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("PinMessageJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        if (mPinMessage) {
            Q_EMIT pinMessageDone();
        } else {
            Q_EMIT unPinMessageDone();
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("PinMessageJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool PinMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool PinMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "PinMessageJob: mMessageId is empty";
        return false;
    }
    return true;
}

QNetworkRequest PinMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mPinMessage ? RestApiUtil::RestApiUrlType::ChatPinMessage : RestApiUtil::RestApiUrlType::ChatUnPinMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QByteArray PinMessageJob::messageId() const
{
    return mMessageId;
}

void PinMessageJob::setMessageId(const QByteArray &messageId)
{
    mMessageId = messageId;
}

bool PinMessageJob::pinMessage() const
{
    return mPinMessage;
}

void PinMessageJob::setPinMessage(bool pinMessage)
{
    mPinMessage = pinMessage;
}

QJsonDocument PinMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["messageId"_L1] = QLatin1StringView(mMessageId);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_pinmessagejob.cpp"
