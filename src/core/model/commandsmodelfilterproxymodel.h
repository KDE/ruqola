/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_TESTS_EXPORT CommandsModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CommandsModelFilterProxyModel(QObject *parent = nullptr);
    ~CommandsModelFilterProxyModel() override;

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
