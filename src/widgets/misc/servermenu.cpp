/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servermenu.h"
#include "accountmanager.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KActionCollection>
#include <KLocalizedString>
#include <QActionGroup>
#include <QMenu>

ServerMenu::ServerMenu(QWidget *parent)
    : KActionMenu(parent)
{
    setText(i18n("Server"));
    connect(menu(), &QMenu::aboutToShow, this, &ServerMenu::slotUpdateAccountMenu);
}

ServerMenu::~ServerMenu() = default;

void ServerMenu::setActionCollection(KActionCollection *ac)
{
    mActionCollection = ac;
}

void ServerMenu::slotUpdateAccountMenu()
{
    menu()->clear();
    auto accountManager = Ruqola::self()->accountManager();
    RocketChatAccountModel *model = accountManager->rocketChatAccountModel();
    const QString currentAccountName = accountManager->currentAccount();
    const int accountNumber = model->accountNumber();
    if (accountNumber == 0) {
        // Nothing
    } else if (accountNumber == 1) {
        auto action = new QAction(model->account(0)->displayName(), this);
        menu()->addAction(action);
        action->setEnabled(false);
    } else {
        auto group = new QActionGroup(this);
        group->setExclusive(true);
        for (int i = 0; i < accountNumber; ++i) {
            RocketChatAccount *account = model->account(i);
            if (account->accountEnabled()) {
                const QString accountName = account->accountName();
                const QString displayName = account->displayName();
                auto action = new QAction(displayName, this);
                action->setCheckable(true);
                group->addAction(action);
                if (currentAccountName == accountName) {
                    action->setChecked(true);
                }
                menu()->addAction(action);
                if (mActionCollection) {
                    mActionCollection->setDefaultShortcut(action, QKeySequence(QStringLiteral("CTRL+%1").arg(i)));
                }
                connect(action, &QAction::triggered, this, [accountName, accountManager]() {
                    accountManager->setCurrentAccount(accountName);
                });
            }
        }
    }
}
