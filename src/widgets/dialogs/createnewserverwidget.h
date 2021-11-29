/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerWidget(QWidget *parent = nullptr);
    ~CreateNewServerWidget() override;

    Q_REQUIRED_RESULT AccountManager::AccountManagerInfo accountInfo();
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void setExistingAccountName(const QStringList &lst);

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    void slotChangeOkButtonEnabled();
    QStringList mNames;

    QLineEdit *const mAccountName;
    QLineEdit *const mServerName;
    QLineEdit *const mUserName;
    KPasswordLineEdit *const mPasswordLineEdit;
    AccountManager::AccountManagerInfo mAccountInfo;
};
