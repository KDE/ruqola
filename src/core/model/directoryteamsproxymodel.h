/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
