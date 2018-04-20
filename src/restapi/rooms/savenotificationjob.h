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

#ifndef SAVENOTIFICATIONJOB_H
#define SAVENOTIFICATIONJOB_H

#include "restapiabstractjob.h"
#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT SaveNotificationJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    explicit SaveNotificationJob(QObject *parent = nullptr);
    ~SaveNotificationJob() override;

    bool start() override;
    bool requireHttpAuthentication() const override;
    bool canStart() const override;
    QNetworkRequest request() const override;

    QJsonDocument json() const;

    QString roomId() const;
    void setRoomId(const QString &roomId);

    bool disableNotifications() const;
    void setDisableNotifications(bool disableNotifications);

    bool hideUnreadStatus() const;
    void setHideUnreadStatus(bool hideUnreadStatus);

    QString emailNotifications() const;
    void setEmailNotifications(const QString &emailNotifications);

    QString audioNotifications() const;
    void setAudioNotifications(const QString &audioNotifications);

    QString mobilePushNotifications() const;
    void setMobilePushNotifications(const QString &mobilePushNotifications);

    QString audioNotificationValue() const;
    void setAudioNotificationValue(const QString &audioNotificationValue);

    int desktopNotificationDuration() const;
    void setDesktopNotificationDuration(int desktopNotificationDuration);

    QString unreadAlert() const;
    void setUnreadAlert(const QString &unreadAlert);

Q_SIGNALS:
    void changeNotificationDone();

private:
    Q_DISABLE_COPY(SaveNotificationJob)
    void slotChangeNotificationFinished();
    QString mRoomId;

    QString mEmailNotifications;
    QString mAudioNotifications;
    QString mMobilePushNotifications;
    QString mAudioNotificationValue;
    QString mUnreadAlert;
    int mDesktopNotificationDuration = 0;
    bool mDisableNotifications = false;
    bool mHideUnreadStatus = false;
};

#endif // SAVENOTIFICATIONJOB_H
