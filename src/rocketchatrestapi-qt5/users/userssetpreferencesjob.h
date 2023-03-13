/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt5_export.h"
#include "restapiabstractjob.h"
#include <QDebug>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT5_EXPORT UsersSetPreferencesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT5_EXPORT UsersSetPreferencesInfo {
        enum State {
            Unknown = 0,
            Checked = 1,
            Unchecked = 2,
        };

        Q_REQUIRED_RESULT bool isValid() const;
        Q_REQUIRED_RESULT static bool convertToBool(State state);
        Q_REQUIRED_RESULT static State convertToState(bool checked);
        QString userId;
        QString newRoomNotification;
        QString newMessageNotification;
        QString desktopNotifications;
        QString pushNotifications;
        QString emailNotificationMode;
        QStringList highlights;
        State useEmoji = Unknown;
        State convertAsciiToEmoji = Unknown;
        State hideRoles = Unknown;
        State displayAvatars = Unknown;
        State sidebarDisplayAvatar = Unknown;
        State sidebarShowUnread = Unknown;
        State sidebarShowFavorites = Unknown;
        State receiveLoginDetectionEmail = Unknown;
        QString sidebarSortby;
        // Add more
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

protected:
    Q_REQUIRED_RESULT QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    Q_DISABLE_COPY(UsersSetPreferencesJob)
    void onPostRequestResponse(const QJsonDocument &replyJson) override;
    UsersSetPreferencesInfo mUsersSetPreferencesInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo, Q_MOVABLE_TYPE);
LIBROCKETCHATRESTAPI_QT5_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t);
