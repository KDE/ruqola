/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "localdatabase/localmessagedatabase.h"
#include <QWidget>
class RocketChatAccount;
class MessageListView;
class QLineEdit;
class MessageModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExploreDatabaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ExploreDatabaseWidget() override;

private:
    void slotLoad();
    RocketChatAccount *const mRocketChatAccount;
    MessageListView *const mMessageListView;
    std::unique_ptr<LocalMessageDatabase> mLocalMessageDatabase;
    QLineEdit *const mAccountName;
    QLineEdit *const mRoomName;
    MessageModel *const mMessageModel;
};
