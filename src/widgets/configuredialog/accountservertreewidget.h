/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QTreeWidget>
class AccountServerListWidgetItem : public QTreeWidgetItem
{
public:
    explicit AccountServerListWidgetItem(QTreeWidget *parent = nullptr);
    ~AccountServerListWidgetItem() override;
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &accountInfo);

    [[nodiscard]] bool newAccount() const;
    void setNewAccount(bool newAccount);

private:
    AccountManager::AccountManagerInfo mInfo;
    bool mNewAccount = false;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountServerTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AccountServerTreeWidget(QWidget *parent = nullptr);
    ~AccountServerTreeWidget() override;

    void load();
    void save();
    void addAccountConfig();
    void deleteAccountConfig(QTreeWidgetItem *item, bool removeLogs);

    void modifyAccountConfig();

    void slotMoveAccountUp();
    void slotMoveAccountDown();

private:
    // AccountName, remove logs
    QMap<QString, bool> mListRemovedAccount;
};
