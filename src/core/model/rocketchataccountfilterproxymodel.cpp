/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodel.h"
#include "rocketchataccountmodel.h"

RocketChatAccountFilterProxyModel::RocketChatAccountFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(RocketChatAccountModel::Name);
    sort(0);
}

RocketChatAccountFilterProxyModel::~RocketChatAccountFilterProxyModel() = default;

bool RocketChatAccountFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (!mAccountOrder.isEmpty()) {
        if (left.isValid() && right.isValid()) {
            const QString leftString = sourceModel()->data(left, RocketChatAccountModel::AccountName).toString();
            const QString rightString = sourceModel()->data(right, RocketChatAccountModel::AccountName).toString();
            const int leftAccountIndex = mAccountOrder.indexOf(leftString);
            const int rightAccountIndex = mAccountOrder.indexOf(rightString);
            if (leftAccountIndex < rightAccountIndex) {
                return true;
            } else if (leftAccountIndex > rightAccountIndex) {
                return false;
            }
        }
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

QStringList RocketChatAccountFilterProxyModel::accountOrder() const
{
    return mAccountOrder;
}

void RocketChatAccountFilterProxyModel::setAccountOrder(const QStringList &newAccountOrder)
{
    if (mAccountOrder != newAccountOrder) {
        mAccountOrder = newAccountOrder;
        invalidate();
    }
}

#include "moc_rocketchataccountfilterproxymodel.cpp"
