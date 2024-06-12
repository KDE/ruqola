/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AppsMarketPlaceFilterProxyModel(QObject *parent = nullptr);
    ~AppsMarketPlaceFilterProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
