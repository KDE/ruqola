/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfofilterproxymodel.h"
#include "config-ruqola.h"

#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

ModerationReportInfoFilterProxyModel::ModerationReportInfoFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

ModerationReportInfoFilterProxyModel::~ModerationReportInfoFilterProxyModel() = default;

void ModerationReportInfoFilterProxyModel::setFilterString(const QString &string)
{
#if HAVE_TEXT_UTILS
    mFilterString = TextUtils::ConvertText::normalize(string);
#else
    mFilterString = string;
#endif
    invalidate();
}

bool ModerationReportInfoFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    auto match = [&](int role) {
        if (mFilterString.isEmpty()) {
            return true;
        };
#if HAVE_TEXT_UTILS
        const QString str = TextUtils::ConvertText::normalize(modelIndex.data(role).toString());
        return str.contains(mFilterString, Qt::CaseInsensitive);
#else
        const QString str = modelIndex.data(role).toString();
        return str.contains(mFilterString, Qt::CaseInsensitive);
#endif
    };
#if 0
    if (!match(NotificationHistoryModel::RoomName) && !match(NotificationHistoryModel::AccountName) && !match(NotificationHistoryModel::SenderName)
        && !match(NotificationHistoryModel::MessageStr)) {
        return false;
    }
#endif
    return true;
}

#include "moc_moderationreportinfofilterproxymodel.cpp"
