/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QSortFilterProxyModel>

#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT RocketChatAccountFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RocketChatAccountFilterProxyModel(QObject *parent = nullptr);

    ~RocketChatAccountFilterProxyModel() override;

    Q_REQUIRED_RESULT QStringList accountOrder() const;
    void setAccountOrder(const QStringList &newAccountOrder);

protected:
    Q_REQUIRED_RESULT bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    QStringList mAccountOrder;
};
