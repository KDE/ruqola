/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "searchtreebasefilterproxymodel.h"
class LIBRUQOLACORE_EXPORT DirectoryRoomsProxyModel : public SearchTreeBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit DirectoryRoomsProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~DirectoryRoomsProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
