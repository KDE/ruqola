/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef USERSSETPREFERENCESJOB_H
#define USERSSETPREFERENCESJOB_H

#include "restapiabstractjob.h"
#include "librocketchatrestapi-qt5_export.h"
#include <QDebug>

namespace RocketChatRestApi {
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersSetPreferencesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UsersSetPreferencesInfo {
        Q_REQUIRED_RESULT bool isValid() const;
        QString userId;
        QString newRoomNotification;
        QString newMessageNotification;
        QString desktopNotifications;
        QString mobileNotifications;
        QString emailNotificationMode;
        QStringList highlights;
        //Add more
    };

    explicit UsersSetPreferencesJob(QObject *parent = nullptr);
    ~UsersSetPreferencesJob() override;

    Q_REQUIRED_RESULT bool start() override;
    Q_REQUIRED_RESULT bool requireHttpAuthentication() const override;
    Q_REQUIRED_RESULT bool canStart() const override;
    Q_REQUIRED_RESULT QNetworkRequest request() const override;

    Q_REQUIRED_RESULT QJsonDocument json() const;

    Q_REQUIRED_RESULT UsersSetPreferencesInfo usersSetPreferencesInfo() const;
    void setUsersSetPreferencesInfo(const UsersSetPreferencesInfo &usersSetPreferencesInfo);

Q_SIGNALS:
    void usersSetPreferencesDone(const QJsonObject &replyObject);

private:
    Q_DISABLE_COPY(UsersSetPreferencesJob)
    void slotUsersSetPreferences();
    UsersSetPreferencesInfo mUsersSetPreferencesInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator <<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t);

#endif // USERSSETPREFERENCESJOB_H
