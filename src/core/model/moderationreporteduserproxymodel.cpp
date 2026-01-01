/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserproxymodel.h"
#include "moderationreportedusermodel.h"

ModerationReportedUserProxyModel::ModerationReportedUserProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

ModerationReportedUserProxyModel::~ModerationReportedUserProxyModel() = default;

bool ModerationReportedUserProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == ModerationReportedUserModel::ReportDateDisplay) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), ModerationReportedUserModel::ReportDate);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), ModerationReportedUserModel::ReportDate);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}

#include "moc_moderationreporteduserproxymodel.cpp"
