/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingspricecombobox.h"
#include <KLocalizedString>
ApplicationsSettingsPriceComboBox::ApplicationsSettingsPriceComboBox(QWidget *parent)
    : QComboBox(parent)
{
    // TODO Convert to enum
    addItem(i18n("All Prices"), QString());
    addItem(i18n("Free Apps"), QStringLiteral("free"));
    addItem(i18n("Paid Apps"), QStringLiteral("paid"));
    addItem(i18n("Premium"), QStringLiteral("premium"));
}

ApplicationsSettingsPriceComboBox::~ApplicationsSettingsPriceComboBox() = default;

QString ApplicationsSettingsPriceComboBox::currentPrice() const
{
    return currentData().toString();
}

#include "moc_applicationssettingspricecombobox.cpp"
