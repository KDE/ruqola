/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "useinvitetokenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UseInviteTokenJob::UseInviteTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UseInviteTokenJob::~UseInviteTokenJob() = default;

bool UseInviteTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UseInviteTokenJob::start");
    submitPostRequest(json());
    return true;
}

void UseInviteTokenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("UseInviteTokenJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sendInvitationEmailsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("UseInviteTokenJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QStringList UseInviteTokenJob::emails() const
{
    return mEmails;
}

void UseInviteTokenJob::setEmails(const QStringList &newEmails)
{
    mEmails = newEmails;
}

bool UseInviteTokenJob::requireHttpAuthentication() const
{
    return true;
}

bool UseInviteTokenJob::canStart() const
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

QNetworkRequest UseInviteTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SendInvitationEmails);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UseInviteTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["emails"_L1] = QJsonArray::fromStringList(mEmails);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_useinvitetokenjob.cpp"
