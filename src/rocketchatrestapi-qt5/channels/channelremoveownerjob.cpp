/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremoveownerjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelRemoveOwnerJob::ChannelRemoveOwnerJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelRemoveOwnerJob::~ChannelRemoveOwnerJob() = default;

bool ChannelRemoveOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelRemoveOwnerJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelRemoveOwnerJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelRemoveOwnerJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelRemoveOwnerDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelRemoveOwnerJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelRemoveOwnerJob::removeUserId() const
{
    return mRemoveUserId;
}

void ChannelRemoveOwnerJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool ChannelRemoveOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelRemoveOwnerJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveOwnerJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveOwnerJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelRemoveOwnerJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelRemoveOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString ChannelRemoveOwnerJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-user-not-owner")) {
        return i18n("User is not an owner.");
    }

    return RestApiAbstractJob::errorMessage(str, detail);
}
