/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonunicodeproxymodel.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodeutils.h"

using namespace TextEmoticonsCore;
EmoticonUnicodeProxyModel::EmoticonUnicodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonUnicodeModel::Identifier);
    sort(0);
}

EmoticonUnicodeProxyModel::~EmoticonUnicodeProxyModel() = default;

void EmoticonUnicodeProxyModel::clearSearch()
{
    setSearchIdentifier(QString());
}

bool EmoticonUnicodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mCategory.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    if (!mSearchIdentifier.isEmpty()) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString identifier = sourceIndex.data(EmoticonUnicodeModel::Identifier).toString();
        if (identifier.contains(mSearchIdentifier)) {
            return true;
        }
        return false;
    }
    if (mCategory == TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier()) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString identifier = sourceIndex.data(EmoticonUnicodeModel::Identifier).toString();
        if (mRecentEmoticons.contains(identifier)) {
            return true;
        }
    } else {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const auto category = sourceIndex.data(EmoticonUnicodeModel::Category).toString();
        if (mCategory == category) {
            return true;
        }
    }
    return false;
}

QString EmoticonUnicodeProxyModel::searchIdentifier() const
{
    return mSearchIdentifier;
}

void EmoticonUnicodeProxyModel::setSearchIdentifier(const QString &newSearchIdentifier)
{
    if (mSearchIdentifier != newSearchIdentifier) {
        mSearchIdentifier = newSearchIdentifier;
        invalidateFilter();
    }
}

QStringList EmoticonUnicodeProxyModel::recentEmoticons() const
{
    return mRecentEmoticons;
}

void EmoticonUnicodeProxyModel::setRecentEmoticons(const QStringList &newRecentEmoticons)
{
    if (mRecentEmoticons != newRecentEmoticons) {
        mRecentEmoticons = newRecentEmoticons;
        if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == mCategory) {
            invalidate();
        }
    }
}

QString EmoticonUnicodeProxyModel::category() const
{
    return mCategory;
}

void EmoticonUnicodeProxyModel::setCategory(const QString &newCategorie)
{
    if (mCategory != newCategorie) {
        mCategory = newCategorie;
        if (!mSearchIdentifier.isEmpty()) {
            clearSearch();
        } else {
            invalidateFilter();
        }
        if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == mCategory) {
            // Make sure that we reorder recent category
            invalidate();
        }
    }
}

bool EmoticonUnicodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == mCategory) {
        const QString leftIdentifier = sourceModel()->data(left, EmoticonUnicodeModel::Identifier).toString();
        const QString rightIdentifier = sourceModel()->data(right, EmoticonUnicodeModel::Identifier).toString();
        const int positionIdentifierLeft = mRecentEmoticons.indexOf(leftIdentifier);
        const int positionIdentifierRight = mRecentEmoticons.indexOf(rightIdentifier);
        //        qDebug() << " leftIdentifier " << leftIdentifier << " rightIdentifier " << rightIdentifier << " positionIdentifierLeft " <<
        //        positionIdentifierLeft
        //                 << " positionIdentifierRight " << positionIdentifierRight;
        //        qDebug() << "mRecentEmoticons  " << mRecentEmoticons;
        return positionIdentifierLeft < positionIdentifierRight;
    } else {
        const int leftOrder = sourceModel()->data(left, EmoticonUnicodeModel::Order).toInt();
        const int rightOrder = sourceModel()->data(right, EmoticonUnicodeModel::Order).toInt();

        return leftOrder < rightOrder;
    }
}
