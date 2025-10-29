/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountservertreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "activities/activitiesmanager.h"
#include "configurenewserver/createnewserverdialog.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola.h"
#include "ruqolaglobalconfig.h"
#include <KLocalizedString>

#include <QHeaderView>
#include <QPointer>
#include <QTreeWidgetItem>

AccountServerTreeWidget::AccountServerTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragDropMode(QAbstractItemView::InternalMove);
    setRootIsDecorated(false);
    header()->hide();
    setColumnCount(1);
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
        info.displayName = account->settings()->displayName();
        info.accountName = account->accountName();
        info.serverUrl = account->serverUrl();
        info.userName = account->userName();
        info.password = account->settings()->password();
        info.canResetPassword = account->ruqolaServerConfig()->allowPasswordReset() && account->ruqolaServerConfig()->allowPasswordChange();
        info.authenticationInfos = account->authenticationMethodInfos();
        info.authMethodType = account->settings()->authMethodType();
        info.token = account->settings()->authToken();
        info.userId = account->userId();
        info.activitiesSettings = {account->settings()->activities(), account->settings()->activityEnabled()};
        item->setToolTip(0, info.serverUrl);
        item->setNewAccount(false);
        item->setCheckState(0, account->accountEnabled() ? Qt::Checked : Qt::Unchecked);
        item->setAccountInfo(std::move(info));
    }
    resizeColumnToContents(0);
}

void AccountServerTreeWidget::save()
{
    // First remove account
    auto accountManager = Ruqola::self()->accountManager();
    for (const auto &[key, value] : mListRemovedAccount.asKeyValueRange()) {
        accountManager->removeAccount(key, value);
    }

    QStringList order;
    const int numberOfItems(topLevelItemCount());
    order.reserve(numberOfItems);
#if HAVE_ACTIVITY_SUPPORT
    QString currentActivity;
    if (columnCount() == 2) { // Configure activity
        currentActivity = Ruqola::self()->accountManager()->rocketChatAccountProxyModel()->activitiesManager()->currentActivity();
    }
#endif
    // Add account or modify it
    for (int accountIndex = 0; accountIndex < numberOfItems; ++accountIndex) {
        QTreeWidgetItem *it = topLevelItem(accountIndex);
        auto serverListItem = static_cast<AccountServerListWidgetItem *>(it);
        AccountManager::AccountManagerInfo info = serverListItem->accountInfo();

        info.enabled = serverListItem->checkState(0) == Qt::Checked;
#if HAVE_ACTIVITY_SUPPORT
        if (!currentActivity.isEmpty()) { // Configure activity
            info.activitiesSettings.changeActivities(serverListItem->checkState(1) == Qt::Checked, currentActivity);
        }
#endif
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
            newAccountName = u"%1_%2"_s.arg(newAccountName).arg(i);
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
#if HAVE_ACTIVITY_SUPPORT
    setText(1, i18n("Display Account in Current Activity"));
    setCheckState(1,
                  info.activitiesSettings.contains(Ruqola::self()->accountManager()->rocketChatAccountProxyModel()->activitiesManager()->currentActivity())
                      ? Qt::Checked
                      : Qt::Unchecked);
#endif
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
