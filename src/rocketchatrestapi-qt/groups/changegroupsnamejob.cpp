/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsnamejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChangeGroupsNameJob::ChangeGroupsNameJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsNameJob::~ChangeGroupsNameJob() = default;

bool ChangeGroupsNameJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsNameJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsNameJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChangeGroupsNameJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeGroupsnameDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChangeGroupsNameJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeGroupsNameJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsNameJob::canStart() const
{
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsNameJob: name is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsNameJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsNameJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["name"_L1] = name();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsNameJob::name() const
{
    return mName;
}

void ChangeGroupsNameJob::setName(const QString &t)
{
    mName = t;
}

QNetworkRequest ChangeGroupsNameJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRename);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changegroupsnamejob.cpp"
