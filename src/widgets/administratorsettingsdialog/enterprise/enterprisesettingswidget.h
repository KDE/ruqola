/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QLineEdit;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT EnterpriseSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit EnterpriseSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~EnterpriseSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QLineEdit *const mEnterpriseLicense;
    QLabel *const mStatus;
};
