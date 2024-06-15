/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsstatuscombobox.h"
#include <KLocalizedString>

ApplicationsSettingsStatusComboBox::ApplicationsSettingsStatusComboBox(QWidget *parent)
    : QComboBox(parent)
{
    // TODO Convert to enum
    addItem(i18n("All Status"), QString());
    addItem(i18n("Enabled"), QStringLiteral("enabled"));
    addItem(i18n("Disabled"), QStringLiteral("disabled"));
}

ApplicationsSettingsStatusComboBox::~ApplicationsSettingsStatusComboBox() = default;

QString ApplicationsSettingsStatusComboBox::currentStatus() const
{
    return currentData().toString();
}

#include "moc_applicationssettingsstatuscombobox.cpp"
