/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidget.h"

OauthSettingsWidget::OauthSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

OauthSettingsWidget::~OauthSettingsWidget() = default;

void OauthSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    // TODO
}
