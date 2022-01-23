/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct UserCompletionInfo {
        QString username;
        QString userId;
    };
    explicit AddUsersCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddUsersCompletionLineEdit() override;

Q_SIGNALS:
    void newUserName(const AddUsersCompletionLineEdit::UserCompletionInfo &userIno);

private:
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
    RocketChatAccount *const mRocketChatAccount;
};

