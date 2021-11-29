/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct UserCompletionInfo {
        QString username;
        QString userId;
    };
    explicit AddUsersCompletionLineEdit(QWidget *parent = nullptr);
    ~AddUsersCompletionLineEdit() override;

Q_SIGNALS:
    void newUserName(const AddUsersCompletionLineEdit::UserCompletionInfo &userIno);

private:
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
};

