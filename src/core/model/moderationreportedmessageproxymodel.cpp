/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessageproxymodel.h"
#include "moderationreportedmessagemodel.h"

ModerationReportedMessageProxyModel::ModerationReportedMessageProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

ModerationReportedMessageProxyModel::~ModerationReportedMessageProxyModel() = default;

bool ModerationReportedMessageProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == ModerationReportedMessageModel::ReportDateDisplay) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), ModerationReportedMessageModel::ReportDate);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), ModerationReportedMessageModel::ReportDate);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}

#include "moc_moderationreportedmessageproxymodel.cpp"
