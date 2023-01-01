/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagewithdelaylineedit.h"
#include "rocketchataccount.h"
#include <QCompleter>
#include <QStringListModel>
#define MAX_COMPLETION_ITEMS 20
SearchMessageWithDelayLineEdit::SearchMessageWithDelayLineEdit(RocketChatAccount *account, QWidget *parent)
    : SearchWithDelayLineEdit(parent)
    , mCompleter(new QCompleter(this))
    , mCompleterListModel(new QStringListModel(this))
    , mCurrentRocketChatAccount(account)
{
    mCompleter->setObjectName(QStringLiteral("mCompleter"));
    mCompleterListModel->setObjectName(QStringLiteral("mCompleterListModel"));

    mCompleter->setModel(mCompleterListModel);
    setCompleter(mCompleter);

    if (mCurrentRocketChatAccount) {
        const QStringList lst = mCurrentRocketChatAccount->searchListCompletion();
        mCompleterListModel->setStringList(lst);
    }
}

SearchMessageWithDelayLineEdit::~SearchMessageWithDelayLineEdit() = default;

void SearchMessageWithDelayLineEdit::addCompletionItem(const QString &str)
{
    mListCompetion.removeAll(str);
    mListCompetion.prepend(str);
    while (mListCompetion.size() > MAX_COMPLETION_ITEMS) {
        mListCompetion.removeLast();
    }
    mCompleterListModel->setStringList(mListCompetion);
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->setSearchListCompletion(mListCompetion);
    }
}
