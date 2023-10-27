/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "sortfilterproxymodelbase.h"

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryFilterProxyModel : public SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit ServerErrorInfoHistoryFilterProxyModel(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryFilterProxyModel() override;

    void setAccountNameFilter(const QString &newAccountNameFilter);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mAccountNameFilter;
};
