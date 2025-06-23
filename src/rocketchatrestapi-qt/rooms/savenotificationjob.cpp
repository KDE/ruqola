/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "savenotificationjob.h"
using namespace Qt::Literals::StringLiterals;

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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SaveNotificationJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeNotificationDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SaveNotificationJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["roomId"_L1] = QLatin1StringView(mRoomId);
    QJsonObject notificationsJson;

    if (mSettingsWillBeChanged & EmailNotifications) {
        notificationsJson["emailNotifications"_L1] = emailNotifications();
    }
    if (mSettingsWillBeChanged & MobilePushNotifications) {
        notificationsJson["mobilePushNotifications"_L1] = mobilePushNotifications();
    }
    if (mSettingsWillBeChanged & AudioNotificationValue) {
        notificationsJson["audioNotificationValue"_L1] = audioNotificationValue();
    }
    if (mSettingsWillBeChanged & UnreadAlert) {
        notificationsJson["unreadAlert"_L1] = unreadAlert();
    }
    if (mSettingsWillBeChanged & DesktopNotificationDuration) {
        notificationsJson["desktopNotificationDuration"_L1] = desktopNotificationDuration();
    }
    if (mSettingsWillBeChanged & DisableNotifications) {
        notificationsJson["disableNotifications"_L1] = disableNotifications() ? u"1"_s : u"0"_s;
    }
    if (mSettingsWillBeChanged & HideUnreadStatus) {
        notificationsJson["hideUnreadStatus"_L1] = hideUnreadStatus() ? u"1"_s : u"0"_s;
    }
    if (mSettingsWillBeChanged & MuteGroupMentions) {
        notificationsJson["muteGroupMentions"_L1] = muteGroupMentions() ? u"1"_s : u"0"_s;
    }
    if (mSettingsWillBeChanged & DesktopNotification) {
        notificationsJson["desktopNotifications"_L1] = desktopNotifications();
    }
    if (mSettingsWillBeChanged & HideMentionStatus) {
        notificationsJson["hideMentionStatus"_L1] = hideMentionStatus() ? u"1"_s : u"0"_s;
    }
    jsonObj["notifications"_L1] = notificationsJson;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_savenotificationjob.cpp"
