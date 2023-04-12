/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "emoticonunicodeproxymodel.h"
#include "emoticonunicodemodel.h"
#include "emoticonunicodeutils.h"

using namespace TextEmoticonsCore;

class EmoticonUnicodeProxyModel::EmoticonUnicodeProxyModelPrivate
{
public:
    EmoticonUnicodeProxyModelPrivate(EmoticonUnicodeProxyModel *qq)
        : q(qq)
    {
    }
    void clearSearch()
    {
        q->setSearchIdentifier(QString());
    }
    QString mCategory;
    QStringList mRecentEmoticons;
    QString mSearchIdentifier;
    EmoticonUnicodeProxyModel *const q;
};

EmoticonUnicodeProxyModel::EmoticonUnicodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d(new EmoticonUnicodeProxyModel::EmoticonUnicodeProxyModelPrivate(this))
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(EmoticonUnicodeModel::Identifier);
    sort(0);
}

EmoticonUnicodeProxyModel::~EmoticonUnicodeProxyModel() = default;

bool EmoticonUnicodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (d->mCategory.isEmpty()) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    if (!d->mSearchIdentifier.isEmpty()) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString identifier = sourceIndex.data(EmoticonUnicodeModel::Identifier).toString();
        if (identifier.contains(d->mSearchIdentifier)) {
            return true;
        }
        return false;
    }
    if (d->mCategory == TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier()) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const QString identifier = sourceIndex.data(EmoticonUnicodeModel::Identifier).toString();
        if (d->mRecentEmoticons.contains(identifier)) {
            return true;
        }
    } else {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        const auto category = sourceIndex.data(EmoticonUnicodeModel::Category).toString();
        if (d->mCategory == category) {
            return true;
        }
    }
    return false;
}

QString EmoticonUnicodeProxyModel::searchIdentifier() const
{
    return d->mSearchIdentifier;
}

void EmoticonUnicodeProxyModel::setSearchIdentifier(const QString &newSearchIdentifier)
{
    if (d->mSearchIdentifier != newSearchIdentifier) {
        d->mSearchIdentifier = newSearchIdentifier;
        invalidateFilter();
    }
}

QStringList EmoticonUnicodeProxyModel::recentEmoticons() const
{
    return d->mRecentEmoticons;
}

void EmoticonUnicodeProxyModel::setRecentEmoticons(const QStringList &newRecentEmoticons)
{
    if (d->mRecentEmoticons != newRecentEmoticons) {
        d->mRecentEmoticons = newRecentEmoticons;
        if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == d->mCategory) {
            invalidate();
        }
    }
}

QString EmoticonUnicodeProxyModel::category() const
{
    return d->mCategory;
}

void EmoticonUnicodeProxyModel::setCategory(const QString &newCategorie)
{
    if (d->mCategory != newCategorie) {
        d->mCategory = newCategorie;
        if (!d->mSearchIdentifier.isEmpty()) {
            d->clearSearch();
        } else {
            invalidateFilter();
        }
        if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == d->mCategory) {
            // Make sure that we reorder recent category
            invalidate();
        }
    }
}

bool EmoticonUnicodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier() == d->mCategory) {
        const QString leftIdentifier = sourceModel()->data(left, EmoticonUnicodeModel::Identifier).toString();
        const QString rightIdentifier = sourceModel()->data(right, EmoticonUnicodeModel::Identifier).toString();
        const int positionIdentifierLeft = d->mRecentEmoticons.indexOf(leftIdentifier);
        const int positionIdentifierRight = d->mRecentEmoticons.indexOf(rightIdentifier);
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
