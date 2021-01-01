/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef SHOWLISTMESSAGEBASEWIDGET_H
#define SHOWLISTMESSAGEBASEWIDGET_H

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class MessageListView;
class QLabel;
class ListMessagesModelFilterProxyModel;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowListMessageBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowListMessageBaseWidget(QWidget *parent = nullptr);
    ~ShowListMessageBaseWidget() override;

    void setModel(ListMessagesModelFilterProxyModel *model);
    Q_REQUIRED_RESULT MessageListView *messageListView() const;

    void setRoom(Room *room);

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

Q_SIGNALS:
    void loadMoreElements();

private:
    void updateLabel();
    void slotSearchMessageTextChanged(const QString &str);
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    QLineEdit *mSearchMessageLineEdit = nullptr;
    MessageListView *mMessageListView = nullptr;
    QLabel *mMessageListInfo = nullptr;
    ListMessagesModelFilterProxyModel *mModel = nullptr;
};

#endif // SHOWLISTMESSAGEBASEWIDGET_H
