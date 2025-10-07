/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include "messages/message.h"
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RocketChatBackend : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatBackend(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatBackend() override;

    void loadServerInfo();
    /**
     * @brief Adds incoming message from server to appropriate room
     *
     * @param messages The Json containing the message
     */
    void processIncomingMessages(const QJsonArray &messages, bool loadHistory, bool restApi = false);

    void addMessagesFromLocalDataBase(const QList<Message> &messages);

    void removeMessageFromLocalDatabase(const QList<QByteArray> &messageIds, const QByteArray &roomId);

    void ddpLogin();

    void slotRemoved(const QJsonObject &object);
    void slotAdded(const QJsonObject &object);
    void slotChanged(const QJsonObject &object);

    void addMessagesSyncAfterLoadingFromDatabase(QList<Message> messages);

private:
    LIBRUQOLACORE_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotDDPConnectedChanged(bool connected);
    LIBRUQOLACORE_NO_EXPORT void parseServerVersionDone(const QString &version);
    LIBRUQOLACORE_NO_EXPORT void tryAutoLogin();
    LIBRUQOLACORE_NO_EXPORT void subscribeRegistration();
    LIBRUQOLACORE_NO_EXPORT void slotPrivateInfoDone(const QJsonObject &data);
    LIBRUQOLACORE_NO_EXPORT void updateVideoConferenceInfo(const Message &m);
    RocketChatAccount *const mRocketChatAccount;
};
