/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
