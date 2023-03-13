/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsmoderatorsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelsModeratorsJob::ChannelsModeratorsJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelsModeratorsJob::~ChannelsModeratorsJob() = default;

bool ChannelsModeratorsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelsModeratorsJob::start: ");
    submitGetRequest();

    return true;
}

void ChannelsModeratorsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("channelFilesDone success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelFilesDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("channelFilesDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelsModeratorsJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelsModeratorsJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelsModeratorsJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelsModeratorsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsModerators);
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
