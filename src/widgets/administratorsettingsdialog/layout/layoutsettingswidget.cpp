/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "layoutsettingswidget.h"

LayoutSettingsWidget::LayoutSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

LayoutSettingsWidget::~LayoutSettingsWidget() = default;

void LayoutSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    // initializeWidget(mEnabled, mapSettings);
}
