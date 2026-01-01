/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremovemoderatorjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChannelRemoveModeratorJob::ChannelRemoveModeratorJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelRemoveModeratorJob::~ChannelRemoveModeratorJob() = default;

bool ChannelRemoveModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelRemoveModeratorJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelRemoveModeratorJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelRemoveModeratorJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeModeratorDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelRemoveModeratorJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelRemoveModeratorJob::removeUserId() const
{
    return mRemoveUserId;
}

void ChannelRemoveModeratorJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool ChannelRemoveModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelRemoveModeratorJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelRemoveModeratorJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelRemoveModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString ChannelRemoveModeratorJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "error-user-not-moderator"_L1) {
        return i18n("User is not a moderator.");
    }

    return RestApiAbstractJob::errorMessage(str, detail);
}

#include "moc_channelremovemoderatorjob.cpp"
