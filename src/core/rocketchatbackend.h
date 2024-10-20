/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "attachments/file.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include "user.h"
#include <QObject>
class DDPClient;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RocketChatBackend : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatBackend(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatBackend() override;

    /**
     * @brief Adds incoming message from server to appropriate room
     *
     * @param messages The Json containing the message
     */
    void processIncomingMessages(const QJsonArray &messages, bool loadHistory, bool restApi = false);

    void clearUsersList();

    [[nodiscard]] QList<User> users() const;

    void clearFilesList();
    [[nodiscard]] QList<File> files() const;

    void loadPublicSettingsAdministrator(qint64 timeStamp = -1);

    void addMessagesFromLocalDataBase(const QList<Message> &messages);

    void removeMessageFromLocalDatabase(const QList<QByteArray> &messageIds, const QByteArray &roomId);

    void loadPublicSettings(qint64 timeStamp = -1);
    void connectDdpClient();

    void loadPermissionsAdministrator(qint64 timeStamp = -1);

    void loadPrivateSettingsAdministrator(qint64 timeStamp = -1);

private:
    LIBRUQOLACORE_NO_EXPORT void slotRemoved(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotAdded(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotChanged(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotConnectedChanged();
    LIBRUQOLACORE_NO_EXPORT void parseServerVersionDone(const QString &version);
    LIBRUQOLACORE_NO_EXPORT void tryAutoLogin();
    LIBRUQOLACORE_NO_EXPORT void subscribeRegistration();
    LIBRUQOLACORE_NO_EXPORT void slotPrivateInfoDone(const QJsonObject &data);
    LIBRUQOLACORE_NO_EXPORT void updateVideoConferenceInfo(const Message &m);
    LIBRUQOLACORE_NO_EXPORT void initializeSubscription(DDPClient *ddp);

    QList<User> mUsers;
    QList<File> mFiles;
    RocketChatAccount *const mRocketChatAccount;
};
