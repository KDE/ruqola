/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT PersonalAccessTokenInfosFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PersonalAccessTokenInfosFilterProxyModel(QObject *parent = nullptr);
    ~PersonalAccessTokenInfosFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
