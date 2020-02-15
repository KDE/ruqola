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

#include "adduserscompletionlineedit.h"
#include "model/usercompleterfilterproxymodel.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "model/usercompletermodel.h"
#include "common/completionlistview.h"

AddUsersCompletionLineEdit::AddUsersCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
{
    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    setCompletionModel(Ruqola::self()->rocketChatAccount()->userCompleterFilterModelProxy());
    connect(this, &AddUsersCompletionLineEdit::complete, this, &AddUsersCompletionLineEdit::slotComplete);
}

AddUsersCompletionLineEdit::~AddUsersCompletionLineEdit()
{
}

void AddUsersCompletionLineEdit::slotTextChanged(const QString &text)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    //TODO add exception!
    //Add current user + list of users already added.
    rcAccount->userAutocomplete(text, QString());
}

void AddUsersCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(UserCompleterModel::UserName).toString();
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    //FIXME userId or username ?
    Q_EMIT newUserName(completerName);
    clear();
    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
}
