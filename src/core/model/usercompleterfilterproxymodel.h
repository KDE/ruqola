/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT UserCompleterFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit UserCompleterFilterProxyModel(QObject *parent = nullptr);
    ~UserCompleterFilterProxyModel() override;

    void clear();

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

