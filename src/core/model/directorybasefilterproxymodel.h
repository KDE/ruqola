/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class LIBRUQOLACORE_EXPORT DirectoryBaseFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DirectoryBaseFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~DirectoryBaseFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();
};
