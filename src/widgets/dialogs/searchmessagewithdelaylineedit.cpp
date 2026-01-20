/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagewithdelaylineedit.h"

#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QCompleter>
#include <QMenu>
#include <QStringListModel>
#define MAX_COMPLETION_ITEMS 20
using namespace Qt::Literals::StringLiterals;
SearchMessageWithDelayLineEdit::SearchMessageWithDelayLineEdit(RocketChatAccount *account, QWidget *parent)
    : SearchWithDelayLineEdit(parent)
    , mCompleter(new QCompleter(this))
    , mCompleterListModel(new QStringListModel(this))
    , mCurrentRocketChatAccount(account)
{
    auto searchAction = addAction(QIcon::fromTheme(u"edit-find"_s), QLineEdit::LeadingPosition);
    searchAction->setObjectName(u"searchAction"_s);
    searchAction->setToolTip(i18nc("@info:tooltip", "Option"));
    auto optionMenu = new QMenu(this);
    auto regularExpressionAct = optionMenu->addAction(i18n("Regular Expression"));
    regularExpressionAct->setCheckable(true);

    searchAction->setMenu(optionMenu);

#if 0 // TODO
    auto searchExtraAction = addAction(QIcon::fromTheme(u"edit-find"_s), QLineEdit::TrailingPosition);
    searchExtraAction->setObjectName(u"searchExtraAction"_s);
    searchExtraAction->setToolTip(i18nc("@info:tooltip", "Option"));
#endif

    mCompleter->setObjectName(u"mCompleter"_s);
    mCompleterListModel->setObjectName(u"mCompleterListModel"_s);

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

void SearchMessageWithDelayLineEdit::insertSearchString(const QString &str)
{
    if (!text().isEmpty()) {
        insert(u' ' + str);
    } else {
        insert(str);
    }
}

#include "moc_searchmessagewithdelaylineedit.cpp"
