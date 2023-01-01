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
#include "user.h"
#include <QJsonObject>
#include <QObject>

class LocalMessageLogger;
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

    void loadPublicSettings();

private:
    Q_DISABLE_COPY(RocketChatBackend)
    void slotRemoved(const QJsonObject &object);
    void slotAdded(const QJsonObject &object);
    void slotChanged(const QJsonObject &object);
    void slotLoginStatusChanged();
    void slotConnectedChanged();
    void slotUserIDChanged();
    void slotGetServerInfoFailed(bool useDeprecatedVersion);
    void parseServerVersionDone(const QString &version);
    void tryAutoLogin();
    void slotPrivateInfoDone(const QJsonObject &data);

    QVector<User> mUsers;
    QVector<File> mFiles;
    RocketChatAccount *mRocketChatAccount = nullptr;
    std::unique_ptr<LocalMessageLogger> mMessageLogger;
};
