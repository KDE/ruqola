/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "model/directorybasefilterproxymodel.h"

class AdministratorOauthFilterProxyModel : public DirectoryBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorOauthFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorOauthFilterProxyModel() override;

    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
