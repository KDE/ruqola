/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class QTimer;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct UserCompletionInfo {
        QString username;
        QByteArray userId;
    };
    explicit AddUsersCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddUsersCompletionLineEdit() override;

Q_SIGNALS:
    void newUserName(const AddUsersCompletionLineEdit::UserCompletionInfo &userIno);
    void searchRequested(const QString &str);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTimerFired();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    RocketChatAccount *const mRocketChatAccount;
    QTimer *const mSearchTimer;
};
