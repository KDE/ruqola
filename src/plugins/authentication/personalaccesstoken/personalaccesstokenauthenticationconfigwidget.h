/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "plugins/pluginauthenticationconfigurewidget.h"
class QLineEdit;
class PersonalAccessTokenAuthenticationConfigWidget : public PluginAuthenticationConfigureWidget
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent = nullptr);
    ~PersonalAccessTokenAuthenticationConfigWidget() override;

    [[nodiscard]] AccountManager::AccountManagerInfo accountInfo() override;
    void setAccountInfo(const AccountManager::AccountManagerInfo &info) override;
    void changeAuthenticationWidgetStatus(bool enabled) override;

private:
    void slotLoginSettingsChanged();
    QLineEdit *const mServerNameLineEdit;
    QLineEdit *const mAccountNameLineEdit;
    QLineEdit *const mUserLineEdit;
    QLineEdit *const mPersonalAccessTokenLineEdit;
};
