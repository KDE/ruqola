/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "librocketchatrestapi-qt_export.h"
#include "restapiabstractjob.h"
#include <QDebug>

namespace RocketChatRestApi
{
class LIBROCKETCHATRESTAPI_QT_EXPORT UsersSetPreferencesJob : public RestApiAbstractJob
{
    Q_OBJECT
public:
    struct LIBROCKETCHATRESTAPI_QT_EXPORT UsersSetPreferencesInfo {
        enum State {
            Unknown = 0,
            Checked = 1,
            Unchecked = 2,
        };

        [[nodiscard]] bool isValid() const;
        [[nodiscard]] static bool convertToBool(State state);
        [[nodiscard]] static State convertToState(bool checked);
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
        State enableAutoAway = Unknown;
        QString sidebarSortby;
        QString sidebarViewMode;
        int idleTimeLimit = -1;
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
    Q_DISABLE_COPY(UsersSetPreferencesJob)
    LIBROCKETCHATRESTAPI_QT_NO_EXPORT void onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson) override;
    UsersSetPreferencesInfo mUsersSetPreferencesInfo;
};
}
Q_DECLARE_TYPEINFO(RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo, Q_RELOCATABLE_TYPE);
LIBROCKETCHATRESTAPI_QT_EXPORT QDebug operator<<(QDebug d, const RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo &t);
