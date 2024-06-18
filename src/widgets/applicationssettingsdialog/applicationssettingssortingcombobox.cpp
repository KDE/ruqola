/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssortingcombobox.h"
#include <KLocalizedString>
ApplicationsSettingsSortingComboBox::ApplicationsSettingsSortingComboBox(QWidget *parent)
    : QComboBox(parent)
{
    // TODO Convert to enum
    addItem(i18n("A-Z"), QString());
    addItem(i18n("Z-A"), QStringLiteral("free"));
    addItem(i18n("Most recent updated"), QStringLiteral("paid"));
    addItem(i18n("Least recent updated"), QStringLiteral("premium"));
}

ApplicationsSettingsSortingComboBox::~ApplicationsSettingsSortingComboBox() = default;

QString ApplicationsSettingsSortingComboBox::currentPrice() const
{
    return currentData().toString();
}

#include "moc_applicationssettingssortingcombobox.cpp"
