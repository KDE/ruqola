/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setchanneltypejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetChannelTypeJob::SetChannelTypeJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

SetChannelTypeJob::~SetChannelTypeJob() = default;

bool SetChannelTypeJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetChannelTypeJob::start");
    submitPostRequest(json());

    return true;
}

void SetChannelTypeJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SetChannelTypeJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setGroupTypeDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SetChannelTypeJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetChannelTypeJob::GroupType SetChannelTypeJob::type() const
{
    return mType;
}

void SetChannelTypeJob::setType(GroupType type)
{
    mType = type;
}

bool SetChannelTypeJob::requireHttpAuthentication() const
{
    return true;
}

bool SetChannelTypeJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetChannelTypeJob: mRoomId and RoomName are empty";
        return false;
    }
    if (mType == Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetChannelTypeJob: type is not defined";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument SetChannelTypeJob::json() const
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

QNetworkRequest SetChannelTypeJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetType);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_setchanneltypejob.cpp"
