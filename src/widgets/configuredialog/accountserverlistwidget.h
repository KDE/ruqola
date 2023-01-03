/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "dialogs/createnewserverdialog.h"
#include "libruqolawidgets_private_export.h"
#include <QListWidget>
class AccountServerListWidgetItem : public QListWidgetItem
{
public:
    explicit AccountServerListWidgetItem(QListWidget *parent = nullptr);
    ~AccountServerListWidgetItem() override;
    Q_REQUIRED_RESULT AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &accountInfo);

    Q_REQUIRED_RESULT bool newAccount() const;
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
    void deleteAccountConfig(QListWidgetItem *item);

    void modifyAccountConfig();

    void slotMoveAccountUp();
    void slotMoveAccountDown();

private:
    QStringList mListRemovedAccount;
};
