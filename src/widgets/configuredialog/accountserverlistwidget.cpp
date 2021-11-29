/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    RocketChatAccountModel *model = Ruqola::self()->accountManager()->rocketChatAccountModel();
    const int accountNumber = model->accountNumber();
    for (int i = 0; i < accountNumber; ++i) {
        auto item = new AccountServerListWidgetItem(this);
        AccountManager::AccountManagerInfo info;
        info.displayName = model->account(i)->displayName();
        info.accountName = model->account(i)->accountName();
        info.serverUrl = model->account(i)->serverUrl();
        info.userName = model->account(i)->userName();
        info.password = model->account(i)->password();
        item->setAccountInfo(info);
        item->setNewAccount(false);
        item->setCheckState(model->account(i)->accountEnabled() ? Qt::Checked : Qt::Unchecked);
    }
}

void AccountServerListWidget::save()
{
    // First remove account
    for (const QString &accountName : std::as_const(mListRemovedAccount)) {
        Ruqola::self()->accountManager()->removeAccount(accountName);
    }

    // Add account or modify it
    for (int i = 0; i < count(); ++i) {
        QListWidgetItem *it = item(i);
        auto serverListItem = static_cast<AccountServerListWidgetItem *>(it);
        AccountManager::AccountManagerInfo info = serverListItem->accountInfo();

        info.enabled = serverListItem->checkState() == Qt::Checked;
        if (serverListItem->newAccount()) {
            Ruqola::self()->accountManager()->addAccount(info);
        } else {
            Ruqola::self()->accountManager()->modifyAccount(info);
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
