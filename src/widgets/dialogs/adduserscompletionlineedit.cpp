/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserscompletionlineedit.h"
#include "common/completionlistview.h"
#include "common/usercompletiondelegate.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include "rocketchataccount.h"

#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

AddUsersCompletionLineEdit::AddUsersCompletionLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mRocketChatAccount(account)
    , mSearchTimer(new QTimer(this))
{
    if (mRocketChatAccount) {
        setCompletionModel(mRocketChatAccount->userCompleterFilterProxyModel());
    }
    auto delegate = new UserCompletionDelegate(mCompletionListView);
    delegate->setRocketChatAccount(mRocketChatAccount);
    mCompletionListView->setItemDelegate(delegate);

    connect(mSearchTimer, &QTimer::timeout, this, &AddUsersCompletionLineEdit::slotSearchTimerFired);
    connect(this, &AddUsersCompletionLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotSearchTextEdited);

    connect(this, &AddUsersCompletionLineEdit::complete, this, &AddUsersCompletionLineEdit::slotComplete);
}

AddUsersCompletionLineEdit::~AddUsersCompletionLineEdit() = default;

void AddUsersCompletionLineEdit::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(1s);
}

void AddUsersCompletionLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    if (!text().trimmed().isEmpty()) {
        slotTextChanged(text());
    } else {
        clear();
    }
}

void AddUsersCompletionLineEdit::slotTextChanged(const QString &text)
{
    // TODO add exception!
    // Add current user + list of users already added.
    mRocketChatAccount->userAutocomplete(text, QString());
}

void AddUsersCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(UserCompleterModel::UserName).toString();
    const QString userId = index.data(UserCompleterModel::UserId).toString();
    UserCompletionInfo info;
    info.username = completerName;
    info.userId = userId;
    mCompletionListView->hide();
    disconnect(this, &AddUsersCompletionLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT newUserName(info);
    clear();
    connect(this, &AddUsersCompletionLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotSearchTextEdited);
}

#include "moc_adduserscompletionlineedit.cpp"
