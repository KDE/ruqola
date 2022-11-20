/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidget.h"
#include "rocketchataccount.h"

CasSettingsWidget::CasSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
{
}

CasSettingsWidget::~CasSettingsWidget() = default;

void CasSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
