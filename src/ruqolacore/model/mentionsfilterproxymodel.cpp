/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "mentionsfilterproxymodel.h"
#include "mentionsmodel.h"
MentionsFilterProxyModel::MentionsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(MentionsModel::OriginalMessage);
    setSortRole(MentionsModel::SortByTimeStamp);
    sort(0, Qt::AscendingOrder);
}

MentionsFilterProxyModel::~MentionsFilterProxyModel()
{
}

QHash<int, QByteArray> MentionsFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

void MentionsFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

int MentionsFilterProxyModel::total() const
{
    return static_cast<MentionsModel *>(sourceModel())->total();
}
