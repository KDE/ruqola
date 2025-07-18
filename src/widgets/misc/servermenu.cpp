/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servermenu.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
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
    auto *model = accountManager->rocketChatAccountProxyModel();
    const QString currentAccountName = accountManager->currentAccount();
    const int accountNumber = model->rowCount();
    if (accountNumber == 0) {
        // Nothing
    } else if (accountNumber == 1) {
        const auto index = model->index(0, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        auto action = new QAction(account->settings()->displayName(), this);
        menu()->addAction(action);
        action->setEnabled(false);
    } else {
        auto group = new QActionGroup(this);
        group->setExclusive(true);
        for (int i = 0; i < accountNumber; ++i) {
            const auto index = model->index(i, 0);
            auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
            if (account->accountEnabled()) {
                const QString accountName = account->settings()->accountName();
                const QString displayName = account->settings()->displayName();
                auto action = new QAction(displayName, this);
                action->setCheckable(true);
                group->addAction(action);
                if (currentAccountName == accountName) {
                    action->setChecked(true);
                }
                menu()->addAction(action);
                if (mActionCollection) {
                    mActionCollection->setDefaultShortcut(action, QKeySequence(u"CTRL+%1"_s.arg(i)));
                }
                connect(action, &QAction::triggered, this, [accountName, accountManager]() {
                    accountManager->setCurrentAccount(accountName);
                });
            }
        }
    }
}

#include "moc_servermenu.cpp"
