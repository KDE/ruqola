/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PasswordSettingsWidget() override;
    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnablePasswordHistory;
    QSpinBox *const mPasswordHistoryLength;
    QCheckBox *const mEnablePasswordPolicy;
};
