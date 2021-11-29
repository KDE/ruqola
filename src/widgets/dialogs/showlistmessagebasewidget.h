/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
    QLineEdit *const mSearchMessageLineEdit;
    QLabel *const mMessageListInfo;
    MessageListView *const mMessageListView;
    ListMessagesModelFilterProxyModel *mModel = nullptr;
};

