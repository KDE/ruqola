/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "model/searchtreebasefilterproxymodel.h"

class AdministratorOauthFilterProxyModel : public SearchTreeBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorOauthFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorOauthFilterProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
