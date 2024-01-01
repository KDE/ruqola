/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QListWidget>
class AccountServerListWidgetItem : public QListWidgetItem
{
public:
    explicit AccountServerListWidgetItem(QListWidget *parent = nullptr);
    ~AccountServerListWidgetItem() override;
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &accountInfo);

    [[nodiscard]] bool newAccount() const;
    void setNewAccount(bool newAccount);

private:
    AccountManager::AccountManagerInfo mInfo;
    bool mNewAccount = false;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountServerListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit AccountServerListWidget(QWidget *parent = nullptr);
    ~AccountServerListWidget() override;

    void load();
    void save();
    void addAccountConfig();
    void deleteAccountConfig(QListWidgetItem *item, bool removeLogs);

    void modifyAccountConfig();

    void slotMoveAccountUp();
    void slotMoveAccountDown();

private:
    // AccountName, remove logs
    QMap<QString, bool> mListRemovedAccount;
};
