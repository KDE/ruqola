/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "ircfederationwidget.h"

IrcFederationWidget::IrcFederationWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

IrcFederationWidget::~IrcFederationWidget() = default;

void IrcFederationWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    // TODO
}
