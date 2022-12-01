/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "analiticswidget.h"

AnaliticsWidget::AnaliticsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

AnaliticsWidget::~AnaliticsWidget() = default;

void AnaliticsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
