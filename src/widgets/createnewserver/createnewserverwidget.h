/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "accountmanager.h"
#include "authenticationinfo.h"
#include "libruqolawidgets_private_export.h"
class AuthenticationOauthWidget;
class AuthenticationLoginWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerWidget(QWidget *parent = nullptr);
    ~CreateNewServerWidget() override;

    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo();
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void setExistingAccountName(const QStringList &lst);

    void setAuthenticationInfos(const QVector<AuthenticationInfo> &authenticationInfos);
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    AuthenticationOauthWidget *const mAuthenticationWidget;
    AuthenticationLoginWidget *const mAuthenticationLoginWidget;
    AccountManager::AccountManagerInfo mAccountInfo;
};
