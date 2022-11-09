/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mobilesettingswidget.h"

MobileSettingsWidget::MobileSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

MobileSettingsWidget::~MobileSettingsWidget() = default;

void MobileSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
