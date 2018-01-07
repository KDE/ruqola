/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ROCKETCHATBACKEND_H
#define ROCKETCHATBACKEND_H

#include "libruqolacore_export.h"
#include <QObject>
#include <QJsonObject>

#include "roommodel.h"
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT RocketChatBackend : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatBackend(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatBackend();

    /**
    * @brief Adds incoming message from server to appropriate room
    *
    * @param messages The Json containing the message
    */
    void processIncomingMessages(const QJsonArray &messages);

Q_SIGNALS:
    void notification(const QString &title, const QString &message);

private:
    Q_DISABLE_COPY(RocketChatBackend)
    void onAdded(const QJsonObject &object);
    void onChanged(const QJsonObject &object);
    void onLoginStatusChanged();
    void onUserIDChanged();

    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // ROCKETCHATBACKEND_H
