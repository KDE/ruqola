/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceInstalledFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AppsMarketPlaceInstalledFilterProxyModel(QObject *parent = nullptr);
    ~AppsMarketPlaceInstalledFilterProxyModel() override;
};
