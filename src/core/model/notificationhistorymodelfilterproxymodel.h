/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT NotificationHistoryModelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NotificationHistoryModelFilterProxyModel(QObject *parent = nullptr);
    ~NotificationHistoryModelFilterProxyModel() override;

protected:
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};
