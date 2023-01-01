/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountserverlistwidget.h"
#include "accountmanager.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QListWidgetItem>
#include <QPointer>

AccountServerListWidget::AccountServerListWidget(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &AccountServerListWidget::itemDoubleClicked, this, &AccountServerListWidget::modifyAccountConfig);
}

AccountServerListWidget::~AccountServerListWidget() = default;

void AccountServerListWidget::load()
{
    auto model = Ruqola::self()->accountManager()->rocketChatAccountProxyModel();
    const auto accountNumber = model->rowCount();
    for (int i = 0; i < accountNumber; ++i) {
        auto item = new AccountServerListWidgetItem(this);
        const auto index = model->index(i, 0);
        auto account = index.data(RocketChatAccountModel::Account).value<RocketChatAccount *>();
        AccountManager::AccountManagerInfo info;
        info.displayName = account->displayName();
        info.accountName = account->accountName();
        info.serverUrl = account->serverUrl();
        info.userName = account->userName();
        info.password = account->password();
        item->setAccountInfo(info);
        item->setNewAccount(false);
        item->setCheckState(account->accountEnabled() ? Qt::Checked : Qt::Unchecked);
    }
}

void AccountServerListWidget::save()
{
    // First remove account
    auto accountManager = Ruqola::self()->accountManager();
    for (const QString &accountName : std::as_const(mListRemovedAccount)) {
        accountManager->removeAccount(accountName);
    }

    // Add account or modify it
    for (int i = 0; i < count(); ++i) {
        QListWidgetItem *it = item(i);
        auto serverListItem = static_cast<AccountServerListWidgetItem *>(it);
        AccountManager::AccountManagerInfo info = serverListItem->accountInfo();

        info.enabled = serverListItem->checkState() == Qt::Checked;
        if (serverListItem->newAccount()) {
            accountManager->addAccount(info);
        } else {
            accountManager->modifyAccount(info);
        }
    }
}

void AccountServerListWidget::modifyAccountConfig()
{
    QListWidgetItem *item = currentItem();
    if (!item) {
        return;
    }

    auto serverListItem = static_cast<AccountServerListWidgetItem *>(item);
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    dlg->setAccountInfo(serverListItem->accountInfo());
    if (dlg->exec()) {
        const AccountManager::AccountManagerInfo info = dlg->accountInfo();
        serverListItem->setAccountInfo(info);
    }
    delete dlg;
}

void AccountServerListWidget::deleteAccountConfig(QListWidgetItem *item)
{
    mListRemovedAccount.append(item->text());
}

void AccountServerListWidget::addAccountConfig()
{
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    QStringList listAccounts;
    for (int i = 0; i < count(); ++i) {
        QListWidgetItem *it = item(i);
        listAccounts << it->text();
    }
    dlg->setExistingAccountName(listAccounts);
    if (dlg->exec()) {
        AccountManager::AccountManagerInfo info = dlg->accountInfo();
        QStringList accountList;
        accountList.reserve(count());
        for (int i = 0; i < count(); ++i) {
            QListWidgetItem *it = item(i);
            accountList << it->text();
        }
        QString newAccountName = info.accountName;
        int i = 1;
        while (accountList.contains(newAccountName)) {
            newAccountName = QStringLiteral("%1_%2").arg(newAccountName).arg(i);
        }
        info.accountName = newAccountName;
        auto accountServeritem = new AccountServerListWidgetItem(this);
        accountServeritem->setCheckState(Qt::Checked);
        accountServeritem->setAccountInfo(info);
        accountServeritem->setNewAccount(true);
    }
    delete dlg;
}

AccountServerListWidgetItem::AccountServerListWidgetItem(QListWidget *parent)
    : QListWidgetItem(parent)
{
}

AccountServerListWidgetItem::~AccountServerListWidgetItem() = default;

AccountManager::AccountManagerInfo AccountServerListWidgetItem::accountInfo() const
{
    return mInfo;
}

void AccountServerListWidgetItem::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    mInfo = info;
    setText(info.displayName);
}

bool AccountServerListWidgetItem::newAccount() const
{
    return mNewAccount;
}

void AccountServerListWidgetItem::setNewAccount(bool newAccount)
{
    mNewAccount = newAccount;
}
