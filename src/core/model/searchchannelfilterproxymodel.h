/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_TESTS_EXPORT SearchChannelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchChannelFilterProxyModel(QObject *parent = nullptr);
    ~SearchChannelFilterProxyModel() override;

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
