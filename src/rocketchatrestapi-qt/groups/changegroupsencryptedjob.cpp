/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsencryptedjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChangeGroupsEncryptedJob::ChangeGroupsEncryptedJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsEncryptedJob::~ChangeGroupsEncryptedJob() = default;

bool ChangeGroupsEncryptedJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsEncryptedJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsEncryptedJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Change encrypted success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeEncryptedDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Problem when we tried to change encrypted status: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeGroupsEncryptedJob::encrypted() const
{
    return mEncrypted;
}

void ChangeGroupsEncryptedJob::setEncrypted(bool encrypted)
{
    mEncrypted = encrypted;
}

bool ChangeGroupsEncryptedJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsEncryptedJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsEncryptedJob: RoomId is empty";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsEncryptedJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["encrypted"_L1] = encrypted();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChangeGroupsEncryptedJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetEncrypted);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changegroupsencryptedjob.cpp"
