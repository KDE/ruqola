/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createchanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CreateChannelJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        qDebug() << " replyObject : " << replyObject;
        Q_EMIT createChannelDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CreateChannelJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    if (str == "error-duplicate-channel-name"_L1) {
        return i18n("A channel with name '%1' exists", detail.value(u"channel_name"_s).toString());
    } else if (str == "error-invalid-room-name"_L1) {
        return i18n("'%1' is not a valid room name", detail.value(u"channel_name"_s).toString());
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
