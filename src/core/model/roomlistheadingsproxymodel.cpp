/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomlistheadingsproxymodel.h"
#include "ruqola_rooms_debug.h"

#include <QApplication>
#include <QPalette>

RoomListHeadingsProxyModel::RoomListHeadingsProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    std::fill(mSectionCounts.begin(), mSectionCounts.end(), 0);
}

void RoomListHeadingsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    mDirty = true;
    QAbstractItemModel *oldModel = this->sourceModel();
    if (oldModel) {
        disconnect(oldModel, nullptr, this, nullptr);
    }

    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &RoomListHeadingsProxyModel::setDirty);
    connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, &RoomListHeadingsProxyModel::setDirty);
    connect(sourceModel, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &, const QModelIndex &, const QVector<int> &roles) {
        if (roles.empty() || roles.contains(RoomModel::RoomSection))
            setDirty();
    });
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &RoomListHeadingsProxyModel::setDirty);
    QIdentityProxyModel::setSourceModel(sourceModel);

    if (sourceModel) {
        // The handling of persistent model indexes assumes mapToSource can be called for any index
        // This breaks for the extra rows, so we'll have to do it ourselves
        disconnect(sourceModel,
                   SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
                   this,
                   SLOT(_q_sourceLayoutAboutToBeChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
        disconnect(sourceModel,
                   SIGNAL(layoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)),
                   this,
                   SLOT(_q_sourceLayoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint)));
        connect(sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &RoomListHeadingsProxyModel::_our_sourceLayoutAboutToBeChanged);
        connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &RoomListHeadingsProxyModel::_our_sourceLayoutChanged);
    }
}

int RoomListHeadingsProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return QIdentityProxyModel::rowCount() + numVisibleSections();
}

QModelIndex RoomListHeadingsProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    // QIdentityProxyModel uses the proxy row as a source row, we can't do that.
    const int sourceRow = proxyRowToSourceRow(row);
    if (sourceRow == -1) {
        return createIndex(row, column);
    }
    if (sourceRow == -2) {
        // we crash on Logout->Login otherwise
        return {};
    }

    const QModelIndex sourceParent = mapToSource(parent);
    const QModelIndex sourceIndex = sourceModel()->index(sourceRow, column, sourceParent);
    return mapFromSource(sourceIndex);
}

QVariant RoomListHeadingsProxyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());

    const int sourceRow = proxyRowToSourceRow(index.row());

    if (role == IsHeading) {
        return sourceRow == -1;
    }

    if (sourceRow == -1) { // heading
        switch (role) {
        case Qt::DisplayRole:
            return RoomModel::sectionName(proxyRowSection(index.row()));
        case Qt::BackgroundRole:
            return qApp->palette().brush(QPalette::Window);
        default:
            return {};
        }
    }

    return sourceModel()->index(sourceRow, index.column()).data(role);
}

QModelIndex RoomListHeadingsProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid()) {
        return {};
    };
    Q_ASSERT(sourceIndex.model() == sourceModel());
    const int proxyRow = sourceRowToProxyRow(sourceIndex.row());
    return createIndex(proxyRow, sourceIndex.column(), sourceIndex.internalPointer());
}

QModelIndex RoomListHeadingsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid()) {
        return {};
    };
    Q_ASSERT(proxyIndex.model() == this);
    const int sourceRow = proxyRowToSourceRow(proxyIndex.row());
    if (sourceRow == -1) { // title, no source row
        return {};
    };
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    return sourceModel()->createSourceIndex(sourceRow, proxyIndex.column(), proxyIndex.internalPointer());
#else
    // hack, until createSourceIndex exists
    return QIdentityProxyModel::mapToSource(createIndex(sourceRow, proxyIndex.column(), proxyIndex.internalPointer()));
#endif
}

Qt::ItemFlags RoomListHeadingsProxyModel::flags(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid()) {
        return QIdentityProxyModel::flags(proxyIndex);
    }
    Q_ASSERT(proxyIndex.model() == this);
    const int sourceRow = proxyRowToSourceRow(proxyIndex.row());
    if (sourceRow == -1) { // heading, make it non-selectable
        return {};
    };
    return QIdentityProxyModel::flags(proxyIndex);
}

void RoomListHeadingsProxyModel::ensureCachedCounts() const
{
    if (!mDirty) {
        return;
    }
    std::fill(mSectionCounts.begin(), mSectionCounts.end(), 0);
    const auto *model = sourceModel();
    const int rows = model->rowCount();
    int lastSection = 0;
    for (int row = 0; row < rows; ++row) {
        const auto section = model->index(row, 0).data(RoomModel::RoomSection).value<RoomModel::Section>();
        const int sectionId = int(section);
        ++mSectionCounts[sectionId];
        if (sectionId < lastSection) {
            qCWarning(RUQOLA_ROOMS_LOG) << "Unexpected section order in" << sourceModel()->metaObject()->className() << "section" << sectionId << "came after"
                                        << lastSection;
        }
        lastSection = sectionId;
        // qDebug() << row << model->index(row, 0).data().toString() << "section" << RoomModel::sectionName(section);
    }
    mDirty = false;
}

int RoomListHeadingsProxyModel::numVisibleSections() const
{
    ensureCachedCounts();
    auto nonEmpty = [](int count) {
        return count > 0;
    };
    return std::count_if(mSectionCounts.begin(), mSectionCounts.end(), nonEmpty);
}

RoomModel::Section RoomListHeadingsProxyModel::proxyRowSection(int proxyRow) const
{
    ensureCachedCounts();
    // int sourceRow = 0;
    int remaining = proxyRow;
    for (size_t section = 0; section < mSectionCounts.size(); ++section) {
        const int sectionCount = mSectionCounts[section];
        if (sectionCount > 0) {
            if (remaining <= sectionCount) {
                return RoomModel::Section(section);
            }
            // sourceRow += sectionCount;
            remaining -= sectionCount + 1;
        }
    }
    qCWarning(RUQOLA_ROOMS_LOG) << proxyRow << "out of bounds? rowCount=" << rowCount();

    return RoomModel::Section::Unknown;
}

int RoomListHeadingsProxyModel::proxyRowToSourceRow(int proxyRow) const
{
    ensureCachedCounts();
    int sourceRow = 0;
    int remaining = proxyRow;
    for (int sectionCount : mSectionCounts) {
        if (sectionCount > 0) {
            if (remaining == 0) {
                return -1; // title
            }
            if (remaining <= sectionCount) {
                return sourceRow + remaining - 1;
            }
            sourceRow += sectionCount;
            remaining -= sectionCount + 1;
        }
    }
    qCWarning(RUQOLA_ROOMS_LOG) << proxyRow << "out of bounds? rowCount=" << rowCount();
    dumpCache();
    return -2;
}

void RoomListHeadingsProxyModel::dumpCache() const
{
    for (size_t section = 0; section < mSectionCounts.size(); ++section) {
        const int sectionCount = mSectionCounts[section];
        qCDebug(RUQOLA_ROOMS_LOG) << RoomModel::sectionName(RoomModel::Section(section)) << sectionCount;
    }
}

int RoomListHeadingsProxyModel::sourceRowToProxyRow(int sourceRow) const
{
    ensureCachedCounts();
    // ex: [2,3] => 0=>1, 1=>2, 2=>4, 3=>5...
    int proxyRow = 0;
    int remaining = sourceRow;
    for (int sectionCount : mSectionCounts) {
        if (sectionCount > 0) {
            if (remaining < sectionCount) {
                return remaining + proxyRow + 1;
            }
            remaining -= sectionCount;
            proxyRow += sectionCount + 1;
        }
    }
    qCWarning(RUQOLA_ROOMS_LOG) << sourceRow << "out of bounds? rowCount=" << rowCount();
    dumpCache();
    return -1;
}

void RoomListHeadingsProxyModel::setDirty()
{
    mDirty = true;
}

void RoomListHeadingsProxyModel::_our_sourceLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &sourceParents, QAbstractItemModel::LayoutChangeHint hint)
{
    // Same code as in QIdentityProxyModelPrivate::_q_sourceLayoutAboutToBeChanged
    // but with handling of heading rows (no source index = assert in QIdentityProxyModel)
    QList<QPersistentModelIndex> parents;
    parents.reserve(sourceParents.size());
    for (const QPersistentModelIndex &parent : sourceParents) {
        if (!parent.isValid()) {
            parents << QPersistentModelIndex();
            continue;
        }
        const QModelIndex mappedParent = mapFromSource(parent);
        Q_ASSERT(mappedParent.isValid());
        parents << mappedParent;
    }

    Q_EMIT layoutAboutToBeChanged(parents, hint);

    const QModelIndexList persistentIndexes = persistentIndexList();
    mLayoutChangePersistentIndexes.reserve(persistentIndexes.size());

    for (const QModelIndex &proxyIndex : persistentIndexes) {
        Q_ASSERT(proxyIndex.isValid());
        mProxyIndexes << proxyIndex;
        const QPersistentModelIndex srcPersistIndex = mapToSource(proxyIndex);
        // Note that secPersistIndex is invalid for heading rows
        mLayoutChangePersistentIndexes << srcPersistIndex;
    }
}

void RoomListHeadingsProxyModel::_our_sourceLayoutChanged(const QList<QPersistentModelIndex> &sourceParents, QAbstractItemModel::LayoutChangeHint hint)
{
    setDirty();

    // Same code as in QIdentityProxyModelPrivate::_q_sourceLayoutChanged
    // but using our own member variables
    for (int i = 0; i < mProxyIndexes.size(); ++i) {
        const QModelIndex proxyIdx = mProxyIndexes.at(i);
        const QModelIndex newProxyIdx = mapFromSource(mLayoutChangePersistentIndexes.at(i));
        changePersistentIndex(proxyIdx, newProxyIdx);
    }

    mLayoutChangePersistentIndexes.clear();
    mProxyIndexes.clear();

    QList<QPersistentModelIndex> parents;
    parents.reserve(sourceParents.size());
    for (const QPersistentModelIndex &parent : sourceParents) {
        if (!parent.isValid()) {
            parents << QPersistentModelIndex();
            continue;
        }
        const QModelIndex mappedParent = mapFromSource(parent);
        Q_ASSERT(mappedParent.isValid());
        parents << mappedParent;
    }

    Q_EMIT layoutChanged(parents, hint);
}
