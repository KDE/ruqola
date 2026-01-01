/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsdescriptionjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
ChangeGroupsDescriptionJob::ChangeGroupsDescriptionJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsDescriptionJob::~ChangeGroupsDescriptionJob() = default;

bool ChangeGroupsDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsDescriptionJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsDescriptionJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChangeGroupsDescriptionJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeDescriptionDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChangeGroupsDescriptionJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChangeGroupsDescriptionJob::description() const
{
    return mDescription;
}

void ChangeGroupsDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeGroupsDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsDescriptionJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsDescriptionJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj), jsonObj["description"_L1] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChangeGroupsDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changegroupsdescriptionjob.cpp"
