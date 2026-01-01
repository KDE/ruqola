/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssortingcombobox.h"
#include <KLocalizedString>
ApplicationsSettingsSortingComboBox::ApplicationsSettingsSortingComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("A-Z"), AppsMarketPlaceFilterProxyModel::Sorting::AtoZ);
    addItem(i18n("Z-A"), AppsMarketPlaceFilterProxyModel::Sorting::ZtoA);
    addItem(i18n("Most recent updated"), AppsMarketPlaceFilterProxyModel::Sorting::MostRecent);
    addItem(i18n("Least recent updated"), AppsMarketPlaceFilterProxyModel::Sorting::LeastRecent);
}

ApplicationsSettingsSortingComboBox::~ApplicationsSettingsSortingComboBox() = default;

AppsMarketPlaceFilterProxyModel::Sorting ApplicationsSettingsSortingComboBox::currentSorting() const
{
    return currentData().value<AppsMarketPlaceFilterProxyModel::Sorting>();
}

#include "moc_applicationssettingssortingcombobox.cpp"
