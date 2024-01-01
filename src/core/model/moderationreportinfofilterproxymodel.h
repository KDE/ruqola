/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqolacore_export.h"
#include "model/sortfilterproxymodelbase.h"

class LIBRUQOLACORE_EXPORT ModerationReportInfoFilterProxyModel : public SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit ModerationReportInfoFilterProxyModel(QObject *parent = nullptr);
    ~ModerationReportInfoFilterProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
