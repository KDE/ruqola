/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    void setFilterString(const QString &string);

    void setAccountNameFilter(const QString &newAccountNameFilter);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mFilterString;
    QString mAccountNameFilter;
};
