/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UsersSetPreferencesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT UsersSetPreferencesInfo {
        enum State : uint8_t {
            Unknown = 0,
            Checked = 1,
            Unchecked = 2,
        };

        [[nodiscard]] bool isValid() const;
        [[nodiscard]] static bool convertToBool(State state);
        [[nodiscard]] static State convertToState(bool checked);
        QByteArray userId;
        QByteArray newRoomNotification;
        QByteArray newMessageNotification;
        QString desktopNotifications;
        QString pushNotifications;
        QString emailNotificationMode;
        QStringList highlights;
        State useEmoji = State::Unknown;
        State convertAsciiToEmoji = State::Unknown;
        State hideRoles = State::Unknown;
        State displayAvatars = State::Unknown;
        State sidebarDisplayAvatar = State::Unknown;
        State sidebarShowUnread = State::Unknown;
        State sidebarShowFavorites = State::Unknown;
        State receiveLoginDetectionEmail = State::Unknown;
        State enableAutoAway = State::Unknown;
        QString sidebarSortby;
        QString sidebarViewMode;
        int idleTimeLimit = -1;
        int notificationsSoundVolume = -1;
        int voipRingerVolume = -1;
        int masterVolume = -1;
        State muteFocusedConversations = State::Unknown;

        // Add more
    };

    explicit UsersSetPreferencesJob(QObject *parent = nullptr);
    ~UsersSetPreferencesJob() override;

    [[nodiscard]] bool start() override;
    [[nodiscard]] bool requireHttpAuthentication() const override;
    [[nodiscard]] bool canStart() const override;
    [[nodiscard]] QNetworkRequest request() const override;

    [[nodiscard]] QJsonDocument json() const;

    [[nodiscard]] UsersSetPreferencesInfo usersSetPreferencesInfo() const;
    void setUsersSetPreferencesInfo(const UsersSetPreferencesInfo &usersSetPreferencesInfo);

Q_SIGNALS:
    void usersSetPreferencesDone(const QJsonObject &replyObject);

protected:
    [[nodiscard]] QString errorMessage(const QString &str, const QJsonObject &details) override;

private:
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UsersSetPreferencesInfo mUsersSetPreferencesInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t);
