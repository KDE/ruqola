/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT EmoticonCustomModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonCustomModelFilterProxyModel(QObject *parent = nullptr);
    ~EmoticonCustomModelFilterProxyModel() override;

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
