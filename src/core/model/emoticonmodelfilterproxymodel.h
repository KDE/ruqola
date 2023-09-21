/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT EmoticonModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonModelFilterProxyModel(QObject *parent = nullptr);
    ~EmoticonModelFilterProxyModel() override;

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
