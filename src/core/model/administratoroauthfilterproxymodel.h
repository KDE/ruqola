/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "model/searchtreebasefilterproxymodel.h"

class LIBRUQOLACORE_EXPORT AdministratorOauthFilterProxyModel : public SearchTreeBaseFilterProxyModel
{
    Q_OBJECT
public:
    explicit AdministratorOauthFilterProxyModel(QAbstractListModel *model = nullptr, QObject *parent = nullptr);
    ~AdministratorOauthFilterProxyModel() override;

    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
