/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "accountmanager.h"
#include "libruqolawidgets_private_export.h"
#include <QStackedWidget>
class CheckNewServerUrlWidget;
class CreateNewServerWidget;
class PluginAuthenticationConfigureWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateNewServerStackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit CreateNewServerStackWidget(QWidget *parent = nullptr);
    ~CreateNewServerStackWidget() override;

    void setExistingAccountNames(const QStringList &lst);
    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() const;

    void setAccountInfo(const AccountManager::AccountManagerInfo &info);

    void slotConfigureActivities();
    void checkServerUrl(const QString &serverUrl);
Q_SIGNALS:
    void settingsIsValid(bool state);
    void tryLogin();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void addAuthenticationConfigureWidget(AuthenticationManager::AuthMethodType type);
    QStringList mExistingAccountNames;
    AccountManager::AccountManagerInfo mAccountManagerInfo;
    CheckNewServerUrlWidget *const mCheckNewServerUrlWidget;
    CreateNewServerWidget *const mCreateNewServerWidget;
    PluginAuthenticationConfigureWidget *mPluginAuthenticationConfigureWidget = nullptr;
};
