/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidget.h"
#include "rocketchataccount.h"

SlackBridgeWidget::SlackBridgeWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

SlackBridgeWidget::~SlackBridgeWidget() = default;

void SlackBridgeWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
