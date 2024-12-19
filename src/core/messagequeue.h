/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include <QObject>
class RocketChatAccount;
class MessageQueue : public QObject
{
    Q_OBJECT
public:
    explicit MessageQueue(RocketChatAccount *account, QObject *parent = nullptr);
    ~MessageQueue() override;

    /**
     * @brief Retry to send unsent messages in DDPClient's abstract message queue
     */
    void processQueue();

    void loadCache();

private:
    void onLoginStatusChanged();
    RocketChatAccount *const mRocketChatAccount;
};
