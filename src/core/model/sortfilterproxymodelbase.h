/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT SortFilterProxyModelBase : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModelBase(QObject *parent = nullptr);
    ~SortFilterProxyModelBase() override;

    void setFilterString(const QString &string);

protected:
    QString mFilterString;
};
