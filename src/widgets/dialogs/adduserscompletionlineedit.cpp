/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserscompletionlineedit.h"
#include "common/completionlistview.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

AddUsersCompletionLineEdit::AddUsersCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
{
    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    setCompletionModel(Ruqola::self()->rocketChatAccount()->userCompleterFilterModelProxy());
    connect(this, &AddUsersCompletionLineEdit::complete, this, &AddUsersCompletionLineEdit::slotComplete);
}

AddUsersCompletionLineEdit::~AddUsersCompletionLineEdit() = default;

void AddUsersCompletionLineEdit::slotTextChanged(const QString &text)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    // TODO add exception!
    // Add current user + list of users already added.
    rcAccount->userAutocomplete(text, QString());
}

void AddUsersCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(UserCompleterModel::UserName).toString();
    const QString userId = index.data(UserCompleterModel::UserId).toString();
    UserCompletionInfo info;
    info.username = completerName;
    info.userId = userId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    Q_EMIT newUserName(info);
    clear();
    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
}
