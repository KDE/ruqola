/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librestapi_private_export.h"
#include "restapiabstractjob.h"
namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_TESTS_EXPORT SaveNotificationJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    // Since 0.63
    explicit SaveNotificationJob(QObject *parent = nullptr);
    ~SaveNotificationJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    [[nodiscard]] bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    [[nodiscard]] bool muteGroupMentions() const;
    void setMuteGroupMentions(bool muteGroupMentions);

    [[nodiscard]] bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool hideUnreadStatus);

    [[nodiscard]] QString emailNotifications() const;
    void setEmailNotifications(const QString &emailNotifications);

    [[nodiscard]] QString mobilePushNotifications() const;
    void setMobilePushNotifications(const QString &mobilePushNotifications);

    [[nodiscard]] QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    [[nodiscard]] int desktopNotificationDuration() const;
    void setDesktopNotificationDuration(int desktopNotificationDuration);

    [[nodiscard]] QString unreadAlert() const;
    void setUnreadAlert(const QString &unreadAlert);

    [[nodiscard]] QString desktopNotifications() const;
    void setDesktopNotifications(const QString &desktopNotifications);

    [[nodiscard]] bool hideMentionStatus() const;
    void setHideMentionStatus(bool newHideMentionStatus);

Q_SIGNALS:
    void changeNotificationDone();

private:
    Q_DISABLE_COPY(SaveNotificationJob)
    void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;

    enum SettingChanged {
        Unknown = 0,
        EmailNotifications = 1,
        MobilePushNotifications = 2,
        AudioNotificationValue = 4,
        UnreadAlert = 8,
        DesktopNotificationDuration = 16,
        DisableNotifications = 32,
        HideUnreadStatus = 64,
        MuteGroupMentions = 128,
        DesktopNotification = 256,
        HideMentionStatus = 512,
    };
    Q_DECLARE_FLAGS(SettingsChanged, SettingChanged)

    SettingsChanged mSettingsWillBeChanged = SettingChanged::Unknown;

    QString mRoomId;

    QString mDesktopNotifications;
    QString mEmailNotifications;
    QString mMobilePushNotifications;
    QString mAudioNotificationValue;
    QString mUnreadAlert;
    int mDesktopNotificationDuration = 0;
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
    bool mMuteGroupMentions = false;
    bool mHideMentionStatus = false;
};
}
