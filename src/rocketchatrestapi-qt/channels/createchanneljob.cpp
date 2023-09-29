/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createchanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateChannelJob::CreateChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateChannelJob::~CreateChannelJob() = default;

bool CreateChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CreateChannelJob::start");
    submitPostRequest(json());
    return true;
}

void CreateChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("CreateChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        qDebug() << " replyObject : " << replyObject;
        Q_EMIT createChannelDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("CreateChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

CreateChannelTeamInfo CreateChannelJob::createChannelInfo() const
{
    return mCreateChannelInfo;
}

void CreateChannelJob::setCreateChannelInfo(const CreateChannelTeamInfo &createChannelInfo)
{
    mCreateChannelInfo = createChannelInfo;
}

QString CreateChannelJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-duplicate-channel-name")) {
        return i18n("A channel with name '%1' exists", detail.value(QStringLiteral("channel_name")).toString());
    } else if (str == QLatin1String("error-invalid-room-name")) {
        return i18n("'%1' is not a valid room name", detail.value(QStringLiteral("channel_name")).toString());
    }

    return RestApiAbstractJob::errorMessage(str, detail);
}

bool CreateChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateChannelJob::canStart() const
{
    if (!mCreateChannelInfo.canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateChannelJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateChannelJob::json() const
{
    return mCreateChannelInfo.json();
}

QNetworkRequest CreateChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_createchanneljob.cpp"
