/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ircwidget.h"

IrcWidget::IrcWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

IrcWidget::~IrcWidget() = default;

void IrcWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
