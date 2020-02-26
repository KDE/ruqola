/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ROOMHELPER_H
#define ROOMHELPER_H

#include <QObject>
#include <QPointer>
#include "libruqolawidgets_private_export.h"
class MessageListView;
class RocketChatAccount;
class MessageLineWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RoomHelper : public QObject
{
    Q_OBJECT
public:
    explicit RoomHelper(QObject *parent = nullptr);
    ~RoomHelper();

    Q_REQUIRED_RESULT QString messageIdBeingEdited() const;
    void setMessageIdBeingEdited(const QString &messageIdBeingEdited);

    MessageListView *messageListView() const;
    void setMessageListView(MessageListView *messageListView);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    RocketChatAccount *currentRocketChatAccount() const;
    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    void slotEditMessage(const QString &messageId, const QString &text);

    MessageLineWidget *messageLineWidget() const;

    void setMessageLineWidget(MessageLineWidget *messageLineWidget);
    void slotSendMessage(const QString &msg);

private:
    QString mMessageIdBeingEdited;
    QString mRoomId;
    MessageListView *mMessageListView = nullptr;
    MessageLineWidget *mMessageLineWidget = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};

#endif // ROOMHELPER_H
