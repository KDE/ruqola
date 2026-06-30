/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolacore_export.h"
#include "model/sortfilterproxymodelbase.h"

class LIBRUQOLACORE_EXPORT BannedUsersFilterProxyModel : public SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit BannedUsersFilterProxyModel(QObject *parent = nullptr);
    ~BannedUsersFilterProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
