#include "roomlistheadingsproxymodel.h"
#include "roommodel.h"
#include "ruqola_rooms_debug.h"

#include <KLocalizedString>
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
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, &RoomListHeadingsProxyModel::setDirty);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &RoomListHeadingsProxyModel::setDirty);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &RoomListHeadingsProxyModel::setDirty);
    QIdentityProxyModel::setSourceModel(sourceModel);
}

int RoomListHeadingsProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0; // flat model
    return QIdentityProxyModel::rowCount() + numVisibleSections();
}

QModelIndex RoomListHeadingsProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // QIdentityProxyModel uses the proxy row as a source row, we can't do that.
    const int sourceRow = proxyRowToSourceRow(row);
    if (sourceRow == -1)
        return createIndex(row, column);

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
    if (!sourceIndex.isValid())
        return {};
    Q_ASSERT(sourceIndex.model() == sourceModel());
    const int proxyRow = sourceRowToProxyRow(sourceIndex.row());
    return createIndex(proxyRow, sourceIndex.column(), sourceIndex.internalPointer());
}

QModelIndex RoomListHeadingsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return {};
    Q_ASSERT(proxyIndex.model() == this);
    const int sourceRow = proxyRowToSourceRow(proxyIndex.row());
    if (sourceRow == -1) // title, no source row
        return {};
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    return sourceModel()->createSourceIndex(sourceRow, proxyIndex.column(), proxyIndex.internalPointer());
#else
    // hack, until createSourceIndex exists
    return QIdentityProxyModel::mapToSource(createIndex(sourceRow, proxyIndex.column(), proxyIndex.internalPointer()));
#endif
}

Qt::ItemFlags RoomListHeadingsProxyModel::flags(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QIdentityProxyModel::flags(proxyIndex);
    Q_ASSERT(proxyIndex.model() == this);
    const int sourceRow = proxyRowToSourceRow(proxyIndex.row());
    if (sourceRow == -1) // heading, make it non-selectable
        return {};
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
    int sourceRow = 0;
    int remaining = proxyRow;
    for (size_t section = 0; section < mSectionCounts.size(); ++section) {
        const int sectionCount = mSectionCounts[section];
        if (sectionCount > 0) {
            if (remaining <= sectionCount)
                return RoomModel::Section(section);
            sourceRow += sectionCount;
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
            if (remaining == 0)
                return -1; // title
            if (remaining <= sectionCount)
                return sourceRow + remaining - 1;
            sourceRow += sectionCount;
            remaining -= sectionCount + 1;
        }
    }
    qCWarning(RUQOLA_ROOMS_LOG) << proxyRow << "out of bounds? rowCount=" << rowCount();
    dumpCache();
    return -1;
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
            if (remaining < sectionCount)
                return remaining + proxyRow + 1;
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
