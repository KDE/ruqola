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

    /**
     * @brief Constructs QPair<QString,QJsonDocument> object from QJsonObject
     *
     * @param object The Json containing message attributes
     * @return QPair<QString,QJsonDocument>, The pair containing the method and params
     */
    [[nodiscard]] static QPair<QString, QJsonDocument> fromJson(const QJsonObject &object);

    /**
     * @brief Constructs QBytearray from QPair<QString,QJsonDocument> object
     *
     * @param pair The pair containing method and params
     * @return QByteArray, The Json containing message attributes
     */
    [[nodiscard]] static QByteArray serialize(const QPair<QString, QJsonDocument> &pair);

    void loadCache();

private:
    Q_DISABLE_COPY(MessageQueue)
    void onLoginStatusChanged();
    RocketChatAccount *const mRocketChatAccount;
};
