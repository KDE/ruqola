/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagesdatabase.h"
#include <QWidget>
class RocketChatAccount;
class MessageListView;
class ExploreDatabaseLineEdit;
class MessagesModel;
class QSpinBox;
class QDateTimeEdit;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseViewMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseViewMessagesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseViewMessagesWidget() override;

Q_SIGNALS:
    void messagesLoaded(const QList<Message> &listMessages);
    void loadModelFromDataBase(const QString &accountName, const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoad();
    RocketChatAccount *const mRocketChatAccount;
    MessageListView *const mMessageListView;
    LocalMessagesDatabase *const mLocalMessageDatabase;
    ExploreDatabaseLineEdit *const mRoomName;
    QSpinBox *const mNumberOfMessages;
    QCheckBox *const mUseStartDateTime;
    QDateTimeEdit *const mStartDateTime;
    QCheckBox *const mUseEndDateTime;
    QDateTimeEdit *const mEndDateTime;
    MessagesModel *const mMessageModel;
};
