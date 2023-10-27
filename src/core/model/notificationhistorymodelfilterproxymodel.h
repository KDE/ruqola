/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "model/sortfilterproxymodelbase.h"

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT NotificationHistoryModelFilterProxyModel : public SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit NotificationHistoryModelFilterProxyModel(QObject *parent = nullptr);
    ~NotificationHistoryModelFilterProxyModel() override;

    void setAccountNameFilter(const QString &newAccountNameFilter);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mAccountNameFilter;
};
