/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT OauthSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit OauthSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~OauthSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mSignWithApple;
    QLineEdit *const mAccountsOAuthAppleId;
};
