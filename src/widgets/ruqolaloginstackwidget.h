/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>

class CreateNewServerWidget;
class PluginAuthenticationConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginStackWidget(QWidget *parent = nullptr);
    ~RuqolaLoginStackWidget() override;

    void addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type);
    void setExistingAccountNames(const QStringList &lst);
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

private:
    QStringList mExistingAccountNames;
    AccountManager::AccountManagerInfo mAccountManagerInfo;
    CreateNewServerWidget *const mCreateNewServerWidget;
    PluginAuthenticationConfigureWidget *mPluginAuthenticationConfigureWidget = nullptr;
};
