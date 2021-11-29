/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "model/directorybasefilterproxymodel.h"

class AdministratorInvitesFilterProxyModel : public DirectoryBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorInvitesFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorInvitesFilterProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
