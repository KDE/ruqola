/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SaveNotificationJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.63
    explicit SaveNotificationJob(QObject *parent = nullptr);
    ~SaveNotificationJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    Q_REQUIRED_RESULT bool muteGroupMentions() const;
    void setMuteGroupMentions(bool muteGroupMentions);

    Q_REQUIRED_RESULT bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool hideUnreadStatus);

    Q_REQUIRED_RESULT QString emailNotifications() const;
    void setEmailNotifications(const QString &emailNotifications);

    Q_REQUIRED_RESULT QString audioNotifications() const;
    void setAudioNotifications(const QString &audioNotifications);

    Q_REQUIRED_RESULT QString mobilePushNotifications() const;
    void setMobilePushNotifications(const QString &mobilePushNotifications);

    Q_REQUIRED_RESULT QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    Q_REQUIRED_RESULT int desktopNotificationDuration() const;
    void setDesktopNotificationDuration(int desktopNotificationDuration);

    Q_REQUIRED_RESULT QString unreadAlert() const;
    void setUnreadAlert(const QString &unreadAlert);

    Q_REQUIRED_RESULT QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

Q_SIGNALS:
    void changeNotificationDone();

private:
    Q_DISABLE_COPY(SaveNotificationJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;

    enum SettingChanged {
        Unknown = 0,
        EmailNotifications = 1,
        AudioNotifications = 2,
        MobilePushNotifications = 4,
        AudioNotificationValue = 8,
        UnreadAlert = 16,
        DesktopNotificationDuration = 32,
        DisableNotifications = 64,
        HideUnreadStatus = 128,
        MuteGroupMentions = 256,
        DesktopNotification = 512,
    };
    Q_DECLARE_FLAGS(SettingsChanged, SettingChanged)

    SettingsChanged mSettingsWillBeChanged = SettingChanged::Unknown;

    QString mRoomId;

    QString mDesktopNotifications;
    QString mEmailNotifications;
    QString mAudioNotifications;
    QString mMobilePushNotifications;
    QString mAudioNotificationValue;
    QString mUnreadAlert;
    int mDesktopNotificationDuration = 0;
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
    bool mMuteGroupMentions = false;
};
}
