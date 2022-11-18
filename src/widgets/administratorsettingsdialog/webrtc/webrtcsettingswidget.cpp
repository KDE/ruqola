/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidget.h"

WebRtcSettingsWidget::WebRtcSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
{
}

WebRtcSettingsWidget::~WebRtcSettingsWidget() = default;

void WebRtcSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
