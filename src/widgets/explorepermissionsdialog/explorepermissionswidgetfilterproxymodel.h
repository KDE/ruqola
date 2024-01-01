/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "model/sortfilterproxymodelbase.h"

class ExplorePermissionsWidgetFilterProxyModel : public SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit ExplorePermissionsWidgetFilterProxyModel(QObject *parent = nullptr);
    ~ExplorePermissionsWidgetFilterProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
