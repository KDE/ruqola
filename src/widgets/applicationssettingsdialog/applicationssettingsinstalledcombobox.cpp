/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinstalledcombobox.h"
#include <KLocalizedString>

ApplicationsSettingsInstalledComboBox::ApplicationsSettingsInstalledComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("All"), AppsMarketPlaceFilterProxyModel::InstalledApps::AnyInfoApps);
    addItem(i18n("Private"), AppsMarketPlaceFilterProxyModel::InstalledApps::PrivateApps);
    addItem(i18n("Installed"), AppsMarketPlaceFilterProxyModel::InstalledApps::AllInstalledApps);
}

ApplicationsSettingsInstalledComboBox::~ApplicationsSettingsInstalledComboBox() = default;

AppsMarketPlaceFilterProxyModel::InstalledApps ApplicationsSettingsInstalledComboBox::currentInstalledStatus() const
{
    return currentData().value<AppsMarketPlaceFilterProxyModel::InstalledApps>();
}

#include "moc_applicationssettingsinstalledcombobox.cpp"
