/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "attachments/file.h"
#include "libruqolacore_export.h"
#include "messages/message.h"
#include "user.h"
#include <QJsonObject>
#include <QObject>

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

    Q_REQUIRED_RESULT QVector<User> users() const;

    void clearFilesList();
    Q_REQUIRED_RESULT QVector<File> files() const;

    void loadPublicSettingsAdministrator(qint64 timeStamp = -1);

    void addMessagesFromLocalDataBase(const QVector<Message> &messages);

    void removeMessageFromLocalDatabase(const QStringList &messageIds, const QString &roomId);

    void loadPublicSettings(qint64 timeStamp = -1);

private:
    Q_DISABLE_COPY(RocketChatBackend)
    LIBRUQOLACORE_NO_EXPORT void slotRemoved(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotAdded(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotChanged(const QJsonObject &object);
    LIBRUQOLACORE_NO_EXPORT void slotLoginStatusChanged();
    LIBRUQOLACORE_NO_EXPORT void slotConnectedChanged();
    LIBRUQOLACORE_NO_EXPORT void slotUserIDChanged();
    LIBRUQOLACORE_NO_EXPORT void slotGetServerInfoFailed(bool useDeprecatedVersion);
    LIBRUQOLACORE_NO_EXPORT void parseServerVersionDone(const QString &version);
    LIBRUQOLACORE_NO_EXPORT void tryAutoLogin();
    LIBRUQOLACORE_NO_EXPORT void slotPrivateInfoDone(const QJsonObject &data);
    LIBRUQOLACORE_NO_EXPORT void updateVideoConferenceInfo(const Message &m);

    QVector<User> mUsers;
    QVector<File> mFiles;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
