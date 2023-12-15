/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QDialog>

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
class CreateNewServerWidget;
class QPushButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateNewServerDialog(QWidget *parent = nullptr);
    ~CreateNewServerDialog() override;
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);
    void setExistingAccountName(const QStringList &lst);

private:
    void readConfig();
    void writeConfig();
    CreateNewServerWidget *const mNewAccountWidget;
    QPushButton *mOkButton = nullptr;
};
