/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT RocketChatAccountFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RocketChatAccountFilterProxyModel(QObject *parent = nullptr);

    ~RocketChatAccountFilterProxyModel() override;

    [[nodiscard]] QStringList accountOrder() const;
    void setAccountOrder(const QStringList &newAccountOrder);

protected:
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QStringList mAccountOrder;
};
