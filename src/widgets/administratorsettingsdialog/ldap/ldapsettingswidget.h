/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT LDapSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit LDapSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~LDapSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mEnabled;
    QSpinBox *const mPort;
    QLineEdit *const mHost;
    QCheckBox *const mReconnect;
    QCheckBox *const mAuthenticationEnabled;
    QLineEdit *const mAuthenticationUserDN;
    KPasswordLineEdit *const mAuthenticationPassword;
    QSpinBox *const mTimeouts;
    QSpinBox *const mConnectionTimeouts;
    QSpinBox *const mIdleTimeouts;
};
