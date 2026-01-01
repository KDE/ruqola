/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "localdatabase/localmessagesdatabase.h"
#include <QWidget>
#include <memory.h>

class MessageListView;
class QLineEdit;
class MessagesModel;
class QSpinBox;
class LoadDataBaseGui : public QWidget
{
    Q_OBJECT
public:
    explicit LoadDataBaseGui(QWidget *parent = nullptr);
    ~LoadDataBaseGui() override;

private:
    void slotLoad();
    MessageListView *const mMessageListView;
    std::unique_ptr<LocalMessagesDatabase> mLocalMessageDatabase;
    QLineEdit *const mAccountName;
    QLineEdit *const mRoomName;
    QSpinBox *const mNumberElement;
    MessagesModel *const mMessageModel;
};
