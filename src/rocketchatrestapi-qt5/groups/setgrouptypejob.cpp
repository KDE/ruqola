/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setgrouptypejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SetGroupTypeJob: Success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setGroupTypeDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SetGroupTypeJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: mRoomId is empty";
        return false;
    }
    if (mType == Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetGroupTypeJob: type is not defined";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
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
        jsonObj[QLatin1String("type")] = QStringLiteral("c");
        break;
    case Private:
        jsonObj[QLatin1String("type")] = QStringLiteral("p");
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
