/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountservertreewidget.h"
#include "configurenewserver/createnewserverdialog.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolaglobalconfig.h"

#include <QHeaderView>
#include <QPointer>
#include <QTreeWidgetItem>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

AccountServerTreeWidget::AccountServerTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragDropMode(QAbstractItemView::InternalMove);
    setRootIsDecorated(false);
    header()->hide();
    connect(this, &AccountServerTreeWidget::itemDoubleClicked, this, &AccountServerTreeWidget::modifyAccountConfig);
}

AccountServerTreeWidget::~AccountServerTreeWidget() = default;

void AccountServerTreeWidget::load()
{
    auto model = new RocketChatAccountFilterProxyModel(this);
    model->setFilterActivities(false);
    model->setAccountOrder(Ruqola::self()->accountManager()->rocketChatAccountProxyModel()->accountOrder());
    model->setSourceModel(Ruqola::self()->accountManager()->rocketChatAccountModel());

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
        info.canResetPassword = account->allowPasswordReset() && account->allowPasswordChange();
        info.authenticationInfos = account->authenticationMethodInfos();
        info.authMethodType = account->authMethodType();
        info.token = account->authToken();
        info.userId = account->userId();
        info.activities = account->activities();
        item->setToolTip(0, info.serverUrl);
        item->setNewAccount(false);
#if 0
        {
            QWidget *w = new QWidget;
            QHBoxLayout *l = new QHBoxLayout(w);
            l->setContentsMargins({});
            auto enabled = new QCheckBox(this);
            enabled->setChecked(account->accountEnabled());
            l->addWidget(enabled);
            l->addWidget(new QLabel(info.displayName, this));
            l->addWidget(new QCheckBox(QLatin1String("show in activity"), this));
            l->addStretch(1);
            item->setSizeHint(w->sizeHint());
            setItemWidget(item, w);
        }
#endif
        item->setCheckState(0, account->accountEnabled() ? Qt::Checked : Qt::Unchecked);
        item->setAccountInfo(std::move(info));
    }
}

void AccountServerTreeWidget::save()
{
    // First remove account
    auto accountManager = Ruqola::self()->accountManager();
    QMapIterator<QString, bool> i(mListRemovedAccount);
    while (i.hasNext()) {
        i.next();
        accountManager->removeAccount(i.key(), i.value());
    }

    QStringList order;
    const int numberOfItems(topLevelItemCount());
    order.reserve(numberOfItems);
    // Add account or modify it
    for (int i = 0; i < numberOfItems; ++i) {
        QTreeWidgetItem *it = topLevelItem(i);
        auto serverListItem = static_cast<AccountServerListWidgetItem *>(it);
        AccountManager::AccountManagerInfo info = serverListItem->accountInfo();

        info.enabled = serverListItem->checkState(0) == Qt::Checked;
        if (serverListItem->newAccount()) {
            accountManager->addAccount(info);
        } else {
            accountManager->modifyAccount(info);
        }
        order << info.accountName;
    }
    Ruqola::self()->accountManager()->rocketChatAccountProxyModel()->setAccountOrder(order);
    RuqolaGlobalConfig::self()->setAccountOrder(order);
}

void AccountServerTreeWidget::modifyAccountConfig()
{
    QTreeWidgetItem *item = currentItem();
    if (!item) {
        return;
    }

    auto serverListItem = static_cast<AccountServerListWidgetItem *>(item);
    const auto accountInfo = serverListItem->accountInfo();
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    dlg->setAccountInfo(accountInfo);
    if (dlg->exec()) {
        const AccountManager::AccountManagerInfo info = dlg->accountInfo();
        serverListItem->setAccountInfo(std::move(info));
    }
    delete dlg;
}

void AccountServerTreeWidget::deleteAccountConfig(QTreeWidgetItem *item, bool removeLogs)
{
    mListRemovedAccount.insert(item->text(0), removeLogs);
}

void AccountServerTreeWidget::addAccountConfig()
{
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    QStringList listAccounts;
    for (int i = 0; i < topLevelItemCount(); ++i) {
        listAccounts << topLevelItem(i)->text(0);
    }
    dlg->setExistingAccountName(listAccounts);
    if (dlg->exec()) {
        AccountManager::AccountManagerInfo info = dlg->accountInfo();
        QStringList accountList;
        accountList.reserve(topLevelItemCount());
        for (int i = 0; i < topLevelItemCount(); ++i) {
            accountList << topLevelItem(i)->text(0);
        }
        QString newAccountName = info.accountName;
        int i = 1;
        while (accountList.contains(newAccountName)) {
            newAccountName = QStringLiteral("%1_%2").arg(newAccountName).arg(i);
        }
        info.accountName = newAccountName;
        auto accountServeritem = new AccountServerListWidgetItem(this);
        accountServeritem->setCheckState(0, Qt::Checked);
        accountServeritem->setAccountInfo(std::move(info));
        accountServeritem->setNewAccount(true);
    }
    delete dlg;
}

void AccountServerTreeWidget::slotMoveAccountUp()
{
    if (!currentItem()) {
        return;
    }
    const int pos = indexOfTopLevelItem(currentItem());
    blockSignals(true);
    QTreeWidgetItem *item = takeTopLevelItem(pos);
    // now selected item is at idx(idx-1), so
    // insert the other item at idx, ie. above(below).
    insertTopLevelItem(pos - 1, item);
    blockSignals(false);
    setCurrentItem(topLevelItem(pos - 1));
}

void AccountServerTreeWidget::slotMoveAccountDown()
{
    if (!currentItem()) {
        return;
    }
    const int pos = indexOfTopLevelItem(currentItem());
    blockSignals(true);
    QTreeWidgetItem *item = takeTopLevelItem(pos);
    // now selected item is at idx(idx-1), so
    // insert the other item at idx, ie. above(below).
    insertTopLevelItem(pos + 1, item);
    blockSignals(false);
    setCurrentItem(topLevelItem(pos + 1));
}

AccountServerListWidgetItem::AccountServerListWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent)
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
    setText(0, info.displayName);
}

bool AccountServerListWidgetItem::newAccount() const
{
    return mNewAccount;
}

void AccountServerListWidgetItem::setNewAccount(bool newAccount)
{
    mNewAccount = newAccount;
}

#include "moc_accountservertreewidget.cpp"
