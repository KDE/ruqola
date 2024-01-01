/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT EmoticonFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonFilterProxyModel(QObject *parent = nullptr);
    ~EmoticonFilterProxyModel() override;

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
