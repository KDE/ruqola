/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsreadonlyjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChangeGroupsReadonlyJob::ChangeGroupsReadonlyJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsReadonlyJob::~ChangeGroupsReadonlyJob() = default;

bool ChangeGroupsReadonlyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsReadonlyJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsReadonlyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Change read only success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeReadonlyDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Problem when we tried to change read only status: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeGroupsReadonlyJob::readOnly() const
{
    return mReadOnly;
}

void ChangeGroupsReadonlyJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool ChangeGroupsReadonlyJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsReadonlyJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsReadonlyJob: RoomId is empty";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsReadonlyJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["readOnly"_L1] = readOnly();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChangeGroupsReadonlyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetReadOnly);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changegroupsreadonlyjob.cpp"
