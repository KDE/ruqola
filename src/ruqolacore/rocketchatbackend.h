/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2018 Laurent Montel <montel@kde.org>
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

#include "file.h"
#include "libruqolacore_export.h"
#include <QObject>
#include <QJsonObject>
#include "user.h"
#include "model/roommodel.h"
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

    void clearUsersList();

    Q_REQUIRED_RESULT QVector<User *> users() const;

    void clearFilesList();
    Q_REQUIRED_RESULT QVector<File> files() const;

private:
    Q_DISABLE_COPY(RocketChatBackend)
    void slotRemoved(const QJsonObject &object);
    void slotAdded(const QJsonObject &object);
    void slotChanged(const QJsonObject &object);
    void slotLoginStatusChanged();
    void slotConnectedChanged();
    void slotUserIDChanged();
    void slotGetServerInfoFailed(bool useDeprecatedVersion);
    void parseOwnInfoDown(const QJsonObject &replyObject);
    void parseServerVersionDone(const QString &version);

    QVector<User *> mUsers;
    QVector<File> mFiles;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // ROCKETCHATBACKEND_H
