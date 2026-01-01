/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class LIBRUQOLACORE_EXPORT SearchTreeBaseFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    using SortFunction = std::function<bool(const QModelIndex &, const QModelIndex &, bool &)>;

    explicit SearchTreeBaseFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~SearchTreeBaseFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();

    void setSortFunction(SortFunction fn);

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    SortFunction mSortFunction;
};
