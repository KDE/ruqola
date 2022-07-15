/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ldapsettingswidget.h"

LDapSettingsWidget::LDapSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

LDapSettingsWidget::~LDapSettingsWidget()
{
}

void LDapSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
