/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emailsettingswidget.h"

EmailSettingsWidget::EmailSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

EmailSettingsWidget::~EmailSettingsWidget() = default;

void EmailSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
