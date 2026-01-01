/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setgrouptypejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SetGroupTypeJob::SetGroupTypeJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

SetGroupTypeJob::~SetGroupTypeJob() = default;

bool SetGroupTypeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetGroupTypeJob::start");
    submitPostRequest(json());

    return true;
}

void SetGroupTypeJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetGroupTypeJob: Success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setGroupTypeDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetGroupTypeJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetGroupTypeJob::GroupType SetGroupTypeJob::type() const
{
    return mType;
}

void SetGroupTypeJob::setType(GroupType type)
{
    mType = type;
}

bool SetGroupTypeJob::requireHttpAuthentication() const
{
    return true;
}

bool SetGroupTypeJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: mRoomId is empty";
        return false;
    }
    if (mType == Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: type is not defined";
        return false;
    }
    return true;
}

QJsonDocument SetGroupTypeJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    switch (mType) {
    case Public:
        jsonObj["type"_L1] = u"c"_s;
        break;
    case Private:
        jsonObj["type"_L1] = u"p"_s;
        break;
    case Unknown:
        break;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest SetGroupTypeJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetType);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_setgrouptypejob.cpp"
