/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT ServerErrorInfoHistoryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ServerErrorInfoHistoryFilterProxyModel(QObject *parent = nullptr);
    ~ServerErrorInfoHistoryFilterProxyModel() override;

    void setFilterString(const QString &string);
    void setAccountNameFilter(const QString &newAccountNameFilter);

protected:
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mFilterString;
    QString mAccountNameFilter;
};
