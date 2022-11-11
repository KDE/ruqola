/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

ConferenceCallSettingsWidget::ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
{
}

ConferenceCallSettingsWidget::~ConferenceCallSettingsWidget() = default;

void ConferenceCallSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
}
