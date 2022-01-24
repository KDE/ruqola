/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directorybasefilterproxymodel.h"
#include "libruqolacore_export.h"
class LIBRUQOLACORE_EXPORT DirectoryTeamsProxyModel : public DirectoryBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit DirectoryTeamsProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~DirectoryTeamsProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
