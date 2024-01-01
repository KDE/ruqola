/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfofilterproxymodel.h"
#include "config-ruqola.h"
#include "moderationreportinfomodel.h"

#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

ModerationReportInfoFilterProxyModel::ModerationReportInfoFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

ModerationReportInfoFilterProxyModel::~ModerationReportInfoFilterProxyModel() = default;

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
    if (!match(ModerationReportInfoModel::Message)) {
        return false;
    }
    return true;
}

#include "moc_moderationreportinfofilterproxymodel.cpp"
