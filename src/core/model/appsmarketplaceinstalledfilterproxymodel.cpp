/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledfilterproxymodel.h"

AppsMarketPlaceInstalledFilterProxyModel::AppsMarketPlaceInstalledFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

AppsMarketPlaceInstalledFilterProxyModel::~AppsMarketPlaceInstalledFilterProxyModel() = default;

#include "moc_appsmarketplaceinstalledfilterproxymodel.cpp"
