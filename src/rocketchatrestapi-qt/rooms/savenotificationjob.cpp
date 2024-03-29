/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "savenotificationjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
SaveNotificationJob::SaveNotificationJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SaveNotificationJob::~SaveNotificationJob() = default;

bool SaveNotificationJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SaveNotificationJob::start");
    submitPostRequest(json());

    return true;
}

void SaveNotificationJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SaveNotificationJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeNotificationDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SaveNotificationJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool SaveNotificationJob::hideMentionStatus() const
{
    return mHideMentionStatus;
}

void SaveNotificationJob::setHideMentionStatus(bool newHideMentionStatus)
{
    mSettingsWillBeChanged |= HideMentionStatus;
    mHideMentionStatus = newHideMentionStatus;
}

QString SaveNotificationJob::desktopNotifications() const
{
    return mDesktopNotifications;
}

void SaveNotificationJob::setDesktopNotifications(const QString &desktopNotifications)
{
    mSettingsWillBeChanged |= DesktopNotification;
    mDesktopNotifications = desktopNotifications;
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

bool SaveNotificationJob::muteGroupMentions() const
{
    return mMuteGroupMentions;
}

void SaveNotificationJob::setMuteGroupMentions(bool muteGroupMentions)
{
    mSettingsWillBeChanged |= MuteGroupMentions;
    mMuteGroupMentions = muteGroupMentions;
}

QByteArray SaveNotificationJob::roomId() const
{
    return mRoomId;
}

void SaveNotificationJob::setRoomId(const QByteArray &roomId)
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
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SaveNotificationJob: mRoomId is empty";
        return false;
    }
    if (mSettingsWillBeChanged == Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SaveNotificationJob: any settings will be changed! it's a bug";
        return false;
    }
    return true;
}

QNetworkRequest SaveNotificationJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveNotification);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SaveNotificationJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1StringView("roomId")] = QString::fromLatin1(mRoomId);
    QJsonObject notificationsJson;

    if (mSettingsWillBeChanged & EmailNotifications) {
        notificationsJson[QLatin1StringView("emailNotifications")] = emailNotifications();
    }
    if (mSettingsWillBeChanged & MobilePushNotifications) {
        notificationsJson[QLatin1StringView("mobilePushNotifications")] = mobilePushNotifications();
    }
    if (mSettingsWillBeChanged & AudioNotificationValue) {
        notificationsJson[QLatin1StringView("audioNotificationValue")] = audioNotificationValue();
    }
    if (mSettingsWillBeChanged & UnreadAlert) {
        notificationsJson[QLatin1StringView("unreadAlert")] = unreadAlert();
    }
    if (mSettingsWillBeChanged & DesktopNotificationDuration) {
        notificationsJson[QLatin1StringView("desktopNotificationDuration")] = desktopNotificationDuration();
    }
    if (mSettingsWillBeChanged & DisableNotifications) {
        notificationsJson[QLatin1StringView("disableNotifications")] = disableNotifications() ? QStringLiteral("1") : QStringLiteral("0");
    }
    if (mSettingsWillBeChanged & HideUnreadStatus) {
        notificationsJson[QLatin1StringView("hideUnreadStatus")] = hideUnreadStatus() ? QStringLiteral("1") : QStringLiteral("0");
    }
    if (mSettingsWillBeChanged & MuteGroupMentions) {
        notificationsJson[QLatin1StringView("muteGroupMentions")] = muteGroupMentions() ? QStringLiteral("1") : QStringLiteral("0");
    }
    if (mSettingsWillBeChanged & DesktopNotification) {
        notificationsJson[QLatin1StringView("desktopNotifications")] = desktopNotifications();
    }
    if (mSettingsWillBeChanged & HideMentionStatus) {
        notificationsJson[QLatin1StringView("hideMentionStatus")] = hideMentionStatus() ? QStringLiteral("1") : QStringLiteral("0");
    }
    jsonObj[QLatin1StringView("notifications")] = notificationsJson;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_savenotificationjob.cpp"
