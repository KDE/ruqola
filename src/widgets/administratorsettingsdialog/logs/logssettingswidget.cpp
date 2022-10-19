/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "logssettingswidget.h"

LogsSettingsWidget::LogsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

LogsSettingsWidget::~LogsSettingsWidget() = default;

void LogsSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
