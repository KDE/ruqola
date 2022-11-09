/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT TroubleshootSettingsWidget : public SettingsWidgetBase
{
public:
    explicit TroubleshootSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TroubleshootSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;
};
