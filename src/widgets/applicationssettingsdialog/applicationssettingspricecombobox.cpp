/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingspricecombobox.h"

#include <KLocalizedString>
ApplicationsSettingsPriceComboBox::ApplicationsSettingsPriceComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("All Prices"), AppsMarketPlaceFilterProxyModel::Price::AllPrice);
    addItem(i18n("Free Apps"), AppsMarketPlaceFilterProxyModel::Price::Free);
    addItem(i18n("Paid Apps"), AppsMarketPlaceFilterProxyModel::Price::Paid);
    addItem(i18n("Premium"), AppsMarketPlaceFilterProxyModel::Price::Premium);
}

ApplicationsSettingsPriceComboBox::~ApplicationsSettingsPriceComboBox() = default;

AppsMarketPlaceFilterProxyModel::Price ApplicationsSettingsPriceComboBox::currentPrice() const
{
    return currentData().value<AppsMarketPlaceFilterProxyModel::Price>();
}

#include "moc_applicationssettingspricecombobox.cpp"
