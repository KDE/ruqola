/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>
class CreateNewServerCheckUrlWidget;
class CreateNewServerWidget;
class PluginAuthenticationConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerStackWidget(QWidget *parent = nullptr);
    ~CreateNewServerStackWidget() override;

    void setExistingAccountName(const QStringList &lst);
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;

    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

Q_SIGNALS:
    void updateOkButton(bool state);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type);
    CreateNewServerCheckUrlWidget *const mCreateNewServerCheckUrlWidget;
    CreateNewServerWidget *const mCreateNewServerWidget;
    PluginAuthenticationConfigureWidget *mPluginAuthenticationConfigureWidget = nullptr;
};
