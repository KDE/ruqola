/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserscompletionlineedit.h"
using namespace Qt::Literals::StringLiterals;

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
    mSearchTimer->start(200ms);
}

void AddUsersCompletionLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    QString str = text();
    if (!str.trimmed().isEmpty()) {
        if (str.startsWith(u'@')) {
            str.removeFirst();
        }
        if (!str.isEmpty()) {
            slotTextChanged(str);
        } else {
            // We just have @ => hide it
            mCompletionListView->hide();
        }
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
    const QByteArray userId = index.data(UserCompleterModel::UserId).toByteArray();
    UserCompletionInfo info;
    info.username = completerName;
    info.userId = userId;
    if (!info.isValid()) {
        return;
    }
    mCompletionListView->hide();
    disconnect(this, &AddUsersCompletionLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT newUserName(info);
    clear();
    connect(this, &AddUsersCompletionLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotSearchTextEdited);
}

QDebug operator<<(QDebug d, const AddUsersCompletionLineEdit::UserCompletionInfo &t)
{
    d.space() << "userId" << t.userId;
    d.space() << "username" << t.username;
    return d;
}

bool AddUsersCompletionLineEdit::UserCompletionInfo::isValid() const
{
    return !userId.isEmpty() && !username.isEmpty();
}

#include "moc_adduserscompletionlineedit.cpp"
