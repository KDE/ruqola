/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "model/searchtreebasefilterproxymodel.h"

class AdministratorInvitesFilterProxyModel : public SearchTreeBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorInvitesFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorInvitesFilterProxyModel() override;

    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
