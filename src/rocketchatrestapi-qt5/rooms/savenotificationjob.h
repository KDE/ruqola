/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#ifndef SAVENOTIFICATIONJOB_H
#define SAVENOTIFICATIONJOB_H

#include "restapiabstractjob.h"
#include "librestapi_private_export.h"
namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_TESTS_EXPORT SaveNotificationJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    //Since 0.63
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

Q_SIGNALS:
    void changeNotificationDone();

private:
    Q_DISABLE_COPY(SaveNotificationJob)
    void slotChangeNotificationFinished();

    enum SettingChanged
    {
        Unknown = 0,
        EmailNotifications = 1,
        AudioNotifications = 2,
        MobilePushNotifications = 4,
        AudioNotificationValue = 8,
        UnreadAlert = 16,
        DesktopNotificationDuration = 32,
        DisableNotifications = 64,
        HideUnreadStatus = 128
    };
    Q_DECLARE_FLAGS(SettingsChanged, SettingChanged)

    SettingsChanged mSettingsWillBeChanged = SettingChanged::Unknown;

    QString mRoomId;

    //TODO ? desktopNotifications ????
    QString mEmailNotifications;
    QString mAudioNotifications;
    QString mMobilePushNotifications;
    QString mAudioNotificationValue;
    QString mUnreadAlert;
    int mDesktopNotificationDuration = 0;
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
};
}
#endif // SAVENOTIFICATIONJOB_H
