/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsstatuscombobox.h"
#include <KLocalizedString>

ApplicationsSettingsStatusComboBox::ApplicationsSettingsStatusComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem(i18n("All Status"), AppsMarketPlaceFilterProxyModel::Status::AllStatus);
    addItem(i18n("Enabled"), AppsMarketPlaceFilterProxyModel::Status::Enabled);
    addItem(i18n("Disabled"), AppsMarketPlaceFilterProxyModel::Status::Disabled);
}

ApplicationsSettingsStatusComboBox::~ApplicationsSettingsStatusComboBox() = default;

AppsMarketPlaceFilterProxyModel::Status ApplicationsSettingsStatusComboBox::currentStatus() const
{
    return currentData().value<AppsMarketPlaceFilterProxyModel::Status>();
}

#include "moc_applicationssettingsstatuscombobox.cpp"
