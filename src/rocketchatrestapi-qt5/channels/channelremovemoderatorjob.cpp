/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremovemoderatorjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

void ChannelRemoveModeratorJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelRemoveModeratorJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeModeratorDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelRemoveModeratorJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj[QLatin1String("userId")] = removeUserId();

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
    if (str == QLatin1String("error-user-not-moderator")) {
        return i18n("User is not a moderator.");
    }

    return RestApiAbstractJob::errorMessage(str, detail);
}
