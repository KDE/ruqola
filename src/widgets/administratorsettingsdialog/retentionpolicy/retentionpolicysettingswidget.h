/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RetentionPolicySettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit RetentionPolicySettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RetentionPolicySettingsWidget() override;

    void initialize();

private:
    QCheckBox *const mEnabled;
};
