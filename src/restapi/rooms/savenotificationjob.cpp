/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "savenotificationjob.h"
#include "restapimethod.h"
#include "restapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SaveNotificationJob::SaveNotificationJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SaveNotificationJob::~SaveNotificationJob()
{
}

bool SaveNotificationJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("SaveNotificationJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &SaveNotificationJob::slotChangeNotificationFinished);
    return true;
}

void SaveNotificationJob::slotChangeNotificationFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SaveNotificationJob: finished: ") + data);
            Q_EMIT changeNotificationDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to change notifications" << data;
        }
    }
    deleteLater();
}

QString SaveNotificationJob::unreadAlert() const
{
    return mUnreadAlert;
}

void SaveNotificationJob::setUnreadAlert(const QString &unreadAlert)
{
    mSettingsWillBeChanged = mSettingsWillBeChanged & UnreadAlert;
    mUnreadAlert = unreadAlert;
}

int SaveNotificationJob::desktopNotificationDuration() const
{
    return mDesktopNotificationDuration;
}

void SaveNotificationJob::setDesktopNotificationDuration(int desktopNotificationDuration)
{
    mSettingsWillBeChanged |= DesktopNotificationDuration;
    mDesktopNotificationDuration = desktopNotificationDuration;
}

QString SaveNotificationJob::audioNotificationValue() const
{
    return mAudioNotificationValue;
}

void SaveNotificationJob::setAudioNotificationValue(const QString &audioNotificationValue)
{
    mSettingsWillBeChanged |= AudioNotificationValue;
    mAudioNotificationValue = audioNotificationValue;
}

QString SaveNotificationJob::mobilePushNotifications() const
{
    return mMobilePushNotifications;
}

void SaveNotificationJob::setMobilePushNotifications(const QString &mobilePushNotifications)
{
    mSettingsWillBeChanged |= MobilePushNotifications;
    mMobilePushNotifications = mobilePushNotifications;
}

QString SaveNotificationJob::audioNotifications() const
{
    return mAudioNotifications;
}

void SaveNotificationJob::setAudioNotifications(const QString &audioNotifications)
{
    mSettingsWillBeChanged |= AudioNotifications;
    mAudioNotifications = audioNotifications;
}

QString SaveNotificationJob::emailNotifications() const
{
    return mEmailNotifications;
}

void SaveNotificationJob::setEmailNotifications(const QString &emailNotifications)
{
    mSettingsWillBeChanged |= EmailNotifications;
    mEmailNotifications = emailNotifications;
}

bool SaveNotificationJob::hideUnreadStatus() const
{
    return mHideUnreadStatus;
}

void SaveNotificationJob::setHideUnreadStatus(bool hideUnreadStatus)
{
    mSettingsWillBeChanged |= HideUnreadStatus;
    mHideUnreadStatus = hideUnreadStatus;
}

bool SaveNotificationJob::disableNotifications() const
{
    return mDisableNotifications;
}

void SaveNotificationJob::setDisableNotifications(bool disableNotifications)
{
    mSettingsWillBeChanged |= DisableNotifications;
    mDisableNotifications = disableNotifications;
}

QString SaveNotificationJob::roomId() const
{
    return mRoomId;
}

void SaveNotificationJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool SaveNotificationJob::requireHttpAuthentication() const
{
    return true;
}

bool SaveNotificationJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start SaveNotificationJob";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "SaveNotificationJob: mRoomId is empty";
        return false;
    }
    if (mSettingsWillBeChanged == Unknown) {
        qCWarning(RESTAPI_LOG) << "SaveNotificationJob: any settings will be changed! it's a bug";
        return false;
    }
    return true;
}

QNetworkRequest SaveNotificationJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveNotification);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument SaveNotificationJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    QJsonObject notificationsJson;

    if (mSettingsWillBeChanged & EmailNotifications) {
        notificationsJson[QLatin1String("emailNotifications")] = emailNotifications();
    }
    if (mSettingsWillBeChanged & AudioNotifications) {
        notificationsJson[QLatin1String("audioNotifications")] = audioNotifications();
    }
    if (mSettingsWillBeChanged & MobilePushNotifications) {
        notificationsJson[QLatin1String("mobilePushNotifications")] = mobilePushNotifications();
    }
    if (mSettingsWillBeChanged & AudioNotificationValue) {
        notificationsJson[QLatin1String("audioNotificationValue")] = audioNotificationValue();
    }
    if (mSettingsWillBeChanged & UnreadAlert) {
        notificationsJson[QLatin1String("unreadAlert")] = unreadAlert();
    }
    if (mSettingsWillBeChanged & DesktopNotificationDuration) {
        notificationsJson[QLatin1String("desktopNotificationDuration")] = desktopNotificationDuration();
    }
    if (mSettingsWillBeChanged & DisableNotifications) {
        notificationsJson[QLatin1String("disableNotifications")] = disableNotifications() ? QStringLiteral("1") : QStringLiteral("0");
    }
    if (mSettingsWillBeChanged & HideUnreadStatus) {
        notificationsJson[QLatin1String("hideUnreadStatus")] = hideUnreadStatus() ? QStringLiteral("1") : QStringLiteral("0");
    }
    jsonObj[QLatin1String("notifications")] = notificationsJson;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
