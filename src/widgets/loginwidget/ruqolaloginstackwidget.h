/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "authenticationmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>

class PluginAuthenticationConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RuqolaLoginStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit RuqolaLoginStackWidget(QWidget *parent = nullptr);
    ~RuqolaLoginStackWidget() override;

    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;
    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void changeAuthenticationWidgetStatus(bool enabled);

Q_SIGNALS:
    void settingsIsValid(bool state);
    void tryLogin();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type);
    QStringList mExistingAccountNames;
    AccountManager::AccountManagerInfo mAccountManagerInfo;
    PluginAuthenticationConfigureWidget *mPluginAuthenticationConfigureWidget = nullptr;
};
