/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sendinvitationemailjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SendInvitationEmailJob::SendInvitationEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SendInvitationEmailJob::~SendInvitationEmailJob() = default;

bool SendInvitationEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SendInvitationEmailJob::start");
    submitPostRequest(json());
    return true;
}

void SendInvitationEmailJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SendInvitationEmailJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sendInvitationEmailsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SendInvitationEmailJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QStringList SendInvitationEmailJob::emails() const
{
    return mEmails;
}

void SendInvitationEmailJob::setEmails(const QStringList &newEmails)
{
    mEmails = newEmails;
}

bool SendInvitationEmailJob::requireHttpAuthentication() const
{
    return true;
}

bool SendInvitationEmailJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mEmails.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Any email defined";
        return false;
    }
    return true;
}

QNetworkRequest SendInvitationEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SendInvitationEmails);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SendInvitationEmailJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["emails"_L1] = QJsonArray::fromStringList(mEmails);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_sendinvitationemailjob.cpp"
