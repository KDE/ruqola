/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class LIBRUQOLACORE_EXPORT SearchTreeBaseFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchTreeBaseFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~SearchTreeBaseFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();
};
