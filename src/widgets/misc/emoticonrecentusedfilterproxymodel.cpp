/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "emoticonrecentusedfilterproxymodel.h"
#include "model/emoticonmodel.h"
#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>

namespace {
static const char myConfigGroupName[] = "EmoticonRecentUsed";
}
EmoticonRecentUsedFilterProxyModel::EmoticonRecentUsedFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    loadRecentUsed();
}

EmoticonRecentUsedFilterProxyModel::~EmoticonRecentUsedFilterProxyModel()
{
    writeRecentUsed();
}

void EmoticonRecentUsedFilterProxyModel::loadRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QStringList recentUsed = group.readEntry("Recents", QStringList());
    setUsedIdentifier(recentUsed);
}

void EmoticonRecentUsedFilterProxyModel::writeRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Recents", mUsedIdentifier);
    group.sync();
}

QStringList EmoticonRecentUsedFilterProxyModel::usedIdentifier() const
{
    return mUsedIdentifier;
}

void EmoticonRecentUsedFilterProxyModel::setUsedIdentifier(const QStringList &usedIdentifier)
{
    if (mUsedIdentifier != usedIdentifier) {
        mUsedIdentifier = usedIdentifier;
        writeRecentUsed();
        invalidateFilter();
    }
}

void EmoticonRecentUsedFilterProxyModel::addIdentifier(const QString &identifier)
{
    if (!mUsedIdentifier.contains(identifier)) {
        mUsedIdentifier.append(identifier);
        invalidateFilter();
    }
}

bool EmoticonRecentUsedFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString identifier = sourceIndex.data(EmoticonModel::Identifier).toString();
    if (mUsedIdentifier.contains(identifier)) {
        return true;
    }
    return false;
}
