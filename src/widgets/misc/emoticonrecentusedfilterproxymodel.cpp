/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonrecentusedfilterproxymodel.h"
#include "model/emoticonmodel.h"
#include <KConfig>
#include <KConfigGroup>
#include <KSharedConfig>

namespace
{
static const char myEmoticonRecentUsedFilterProxyModelGroupName[] = "EmoticonRecentUsed";
}
EmoticonRecentUsedFilterProxyModel::EmoticonRecentUsedFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    loadRecentUsed();
    sort(0);
}

EmoticonRecentUsedFilterProxyModel::~EmoticonRecentUsedFilterProxyModel()
{
    writeRecentUsed();
}

void EmoticonRecentUsedFilterProxyModel::loadRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedFilterProxyModelGroupName);
    const QStringList recentUsed = group.readEntry("Recents", QStringList());
    setUsedIdentifier(recentUsed);
}

void EmoticonRecentUsedFilterProxyModel::writeRecentUsed()
{
    KConfigGroup group(KSharedConfig::openConfig(), myEmoticonRecentUsedFilterProxyModelGroupName);
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
        invalidate();
    }
}

void EmoticonRecentUsedFilterProxyModel::addIdentifier(const QString &identifier)
{
    if (int i = mUsedIdentifier.indexOf(identifier)) {
        // Remove it for adding in top of list
        if (i != -1) {
            mUsedIdentifier.removeAt(i);
        }
    }
    mUsedIdentifier.prepend(identifier);
    writeRecentUsed();
    invalidate();
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

bool EmoticonRecentUsedFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    const QString identifierLeft = source_left.data(EmoticonModel::Identifier).toString();
    const QString identifierRight = source_right.data(EmoticonModel::Identifier).toString();
    const int indexLeft = mUsedIdentifier.indexOf(identifierLeft);
    const int indexRight = mUsedIdentifier.indexOf(identifierRight);
    return indexLeft < indexRight;
}
