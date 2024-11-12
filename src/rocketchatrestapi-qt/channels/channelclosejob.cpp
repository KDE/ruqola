/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelclosejob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ChannelCloseJob::ChannelCloseJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelCloseJob::~ChannelCloseJob() = default;

bool ChannelCloseJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelCloseJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelCloseJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelCloseJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT closeChannelDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelCloseJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

ChannelCloseJob::ChannelType ChannelCloseJob::channelType() const
{
    return mChannelType;
}

void ChannelCloseJob::setChannelType(ChannelType channelType)
{
    mChannelType = channelType;
}

bool ChannelCloseJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelCloseJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelCloseJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelType == ChannelCloseJob::ChannelType::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelCloseJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelCloseJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelCloseJob::request() const
{
    QUrl url;
    switch (mChannelType) {
    case ChannelType::Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsClose);
        break;
    case ChannelType::Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsClose);
        break;
    case ChannelType::Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImClose);
        break;
    case ChannelType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelCloseJob: Type is not defined";
        break;
    }
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channelclosejob.cpp"
