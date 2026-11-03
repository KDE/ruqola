/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomlistheadingsproxymodel.h"

#include <QApplication>
#include <QPalette>

RoomListHeadingsProxyModel::RoomListHeadingsProxyModel(QObject *parent)
    : QAbstractProxyModel(parent)
{
    rebuildSections();
}

bool RoomListHeadingsProxyModel::isValidSectionRow(int row) const
{
    return row >= 0 && row < int(mSections.size());
}

void RoomListHeadingsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (auto *oldModel = this->sourceModel()) {
        disconnect(oldModel, nullptr, this, nullptr);
    }

    QAbstractProxyModel::setSourceModel(sourceModel);
    mRoomModel = qobject_cast<RoomModel *>(sourceModel);

    if (!sourceModel) {
        rebuildSections();
        endResetModel();
        return;
    }

    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &RoomListHeadingsProxyModel::onRowsInserted);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &RoomListHeadingsProxyModel::onRowsAboutToBeRemoved);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, [this] {
        Q_EMIT layoutAboutToBeChanged();
    });
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, [this] {
        rebuildSections();
        Q_EMIT layoutChanged();
    });

    connect(sourceModel, &QAbstractItemModel::dataChanged, this, &RoomListHeadingsProxyModel::onDataChanged);

    connect(sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &RoomListHeadingsProxyModel::beginResetModel);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, [this]() {
        rebuildSections();
        endResetModel();
    });

    connect(sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &RoomListHeadingsProxyModel::layoutAboutToBeChanged);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, [this]() {
        rebuildSections();
        Q_EMIT layoutChanged();
    });

    if (mRoomModel) {
        // A category is a root row of this model: rebuilding everything is simpler than moving the
        // rooms around by hand, and categories don't change often.
        connect(mRoomModel, &RoomModel::categoriesChanged, this, &RoomListHeadingsProxyModel::resetSections);
    }

    rebuildSections();

    endResetModel();
}

int RoomListHeadingsProxyModel::rowCount(const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
        return int(mSections.size());
    case IndexType::Section:
        if (!isValidSectionRow(parent.row())) {
            return 0;
        }
        return int(mSections.at(parent.row()).rooms.size());
    case IndexType::Channel:
        return 0;
    }
    Q_UNREACHABLE();
}

int RoomListHeadingsProxyModel::columnCount(const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
    case IndexType::Section:
        return 1;
    case IndexType::Channel:
        return 0;
    }
    Q_UNREACHABLE();
}

bool RoomListHeadingsProxyModel::hasChildren(const QModelIndex &index) const
{
    // Don't use the QAbstractProxyModel override which fails on non-proxied indices
    return QAbstractItemModel::hasChildren(index); // NOLINT(bugprone-parent-virtual-call)
}

QModelIndex RoomListHeadingsProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
        if (!isValidSectionRow(row)) {
            return {};
        }
        return createIndex(row, column, sectionMarker);
    case IndexType::Section:
        if (!isValidSectionRow(parent.row()) || row < 0 || row >= int(mSections.at(parent.row()).rooms.size())) {
            return {};
        }
        return createIndex(row, column, parent.row());
    case IndexType::Channel:
        return {};
    }
    Q_UNREACHABLE();
}

QModelIndex RoomListHeadingsProxyModel::parent(const QModelIndex &child) const
{
    switch (type(child)) {
    case IndexType::Root:
    case IndexType::Section:
        return {};
    case IndexType::Channel:
        return createIndex(int(child.internalId()), 0, sectionMarker);
    }
    Q_UNREACHABLE();
}

QVariant RoomListHeadingsProxyModel::data(const QModelIndex &index, int role) const
{
    switch (type(index)) {
    case IndexType::Root:
        return {};
    case IndexType::Section: {
        if (!isValidSectionRow(index.row())) {
            return {};
        }
        const auto &section = mSections.at(index.row());
        switch (role) {
        case Qt::ItemDataRole::DisplayRole:
            if (section.category.isValid()) {
                return section.category.name();
            }
            return RoomModel::sectionName(section.kind);
        case RoomModel::RoomCategory:
            // Empty for a standard section, so that a caller can tell both apart.
            return section.category.identifier();
        case Qt::BackgroundRole:
            return QApplication::palette().brush(QPalette::Window);
        default:
            return {};
        }
    }
    case IndexType::Channel:
        return sourceModel()->data(mapToSource(index), role);
    }
    Q_UNREACHABLE();
}

QModelIndex RoomListHeadingsProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceModel()) {
        return {};
    }

    if (!sourceIndex.isValid()) {
        return {};
    }

    for (auto sectionId = size_t(0), iMax = mSections.size(); sectionId < iMax; ++sectionId) {
        const auto &rooms = mSections.at(sectionId).rooms;

        if (const auto it = std::lower_bound(rooms.cbegin(), rooms.cend(), sourceIndex); it != rooms.cend() && *it == sourceIndex) {
            return createIndex(int(it - rooms.cbegin()), 0, sectionId);
        }
    }

    return {};
}

QModelIndex RoomListHeadingsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!sourceModel()) {
        return {};
    }

    switch (type(proxyIndex)) {
    case IndexType::Root:
    case IndexType::Section:
        return {};
    case IndexType::Channel: {
        const auto sectionId = int(proxyIndex.internalId());
        if (!isValidSectionRow(sectionId)) {
            return {};
        }
        const auto &rooms = mSections.at(sectionId).rooms;
        if (proxyIndex.row() < 0 || proxyIndex.row() >= int(rooms.size())) {
            return {};
        }
        return rooms.at(proxyIndex.row());
    }
    }
    Q_UNREACHABLE();
}

Qt::ItemFlags RoomListHeadingsProxyModel::flags(const QModelIndex &proxyIndex) const
{
    switch (type(proxyIndex)) {
    case IndexType::Root:
        return {};
    case IndexType::Section:
        return Qt::ItemFlag::ItemIsEnabled;
    case IndexType::Channel:
        return QAbstractProxyModel::flags(proxyIndex);
    }
    Q_UNREACHABLE();
}

void RoomListHeadingsProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        const QPersistentModelIndex index = sourceModel()->index(row, 0, parent);
        const auto newSectionId = sectionId(index);
        auto &newSection = mSections.at(newSectionId).rooms;

        const auto newLocation = std::lower_bound(newSection.cbegin(), newSection.cend(), index);
        const auto newLocationRow = int(newLocation - newSection.cbegin());

        beginInsertRows(createIndex(newSectionId, 0, sectionMarker), newLocationRow, newLocationRow);

        newSection.insert(newLocation, index);

        endInsertRows();
    }
}

void RoomListHeadingsProxyModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        const auto index = sourceModel()->index(row, 0, parent);

        const auto ourOldIndex = mapFromSource(index);
        const auto oldSectionId = ourOldIndex.internalId();
        auto &oldSection = mSections.at(oldSectionId).rooms;

        beginRemoveRows(ourOldIndex.parent(), ourOldIndex.row(), ourOldIndex.row());

        oldSection.erase(oldSection.begin() + ourOldIndex.row());

        endRemoveRows();
    }
}

void RoomListHeadingsProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto proxyIndex = mapFromSource(sourceIndex);
        Q_EMIT dataChanged(proxyIndex, proxyIndex, roles);
    }

    if (!roles.empty() && !roles.contains(RoomModel::RoomSection) && !roles.contains(RoomModel::RoomCategory)) {
        return;
    }

    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto ourOldIndex = mapFromSource(sourceIndex);

        const auto oldSectionId = int(ourOldIndex.internalId());
        const auto newSectionId = sectionId(sourceIndex);

        if (oldSectionId == newSectionId) {
            continue;
        }

        auto &oldSection = mSections.at(oldSectionId).rooms;
        auto &newSection = mSections.at(newSectionId).rooms;

        const auto newLocation = std::lower_bound(newSection.cbegin(), newSection.cend(), sourceIndex);
        const auto newLocationRow = int(newLocation - newSection.cbegin());

        beginMoveRows(ourOldIndex.parent(), ourOldIndex.row(), ourOldIndex.row(), createIndex(newSectionId, 0, sectionMarker), newLocationRow);

        auto persistantIndex = oldSection[ourOldIndex.row()];
        oldSection.erase(oldSection.begin() + ourOldIndex.row());
        newSection.insert(newLocation, std::move(persistantIndex));

        endMoveRows();
    }
}

int RoomListHeadingsProxyModel::sectionId(const QModelIndex &sourceIndex) const
{
    const auto kind = sourceIndex.data(RoomModel::RoomSection).value<RoomModel::Section>();
    switch (kind) {
    // As in Rocket.Chat, the dynamic sections come before the category of the room.
    case RoomModel::Section::IncomingCalls:
    case RoomModel::Section::IncomingLivechats:
    case RoomModel::Section::OpenLivechats:
    case RoomModel::Section::OnHoldChats:
    case RoomModel::Section::Unread:
        return mStandardSectionRows.at(uint(kind));
    default:
        break;
    }
    // Otherwise a room which belongs to a category is only listed in that category.
    if (const QByteArray categoryId = sourceIndex.data(RoomModel::RoomCategory).toByteArray(); !categoryId.isEmpty()) {
        const auto sameCategory = [&categoryId](const Section &section) {
            return section.category.identifier() == categoryId;
        };
        if (const auto it = std::find_if(mSections.cbegin(), mSections.cend(), sameCategory); it != mSections.cend()) {
            return int(it - mSections.cbegin());
        }
        // Unknown category: fall back to the standard sections.
    }
    return mStandardSectionRows.at(uint(kind));
}

void RoomListHeadingsProxyModel::resetSections()
{
    beginResetModel();
    rebuildSections();
    endResetModel();
}

void RoomListHeadingsProxyModel::rebuildSections()
{
    mSections.clear();

    const QList<Category> categories = mRoomModel ? mRoomModel->categories() : QList<Category>{};
    mSections.reserve(sectionCount + categories.size());
    for (uint i = 0; i < sectionCount; ++i) {
        const auto kind = RoomModel::Section(i);
        mStandardSectionRows[i] = int(mSections.size());
        Section section;
        section.kind = kind;
        mSections.push_back(std::move(section));
        if (kind == RoomModel::Section::Favorites) {
            for (const Category &category : categories) {
                Section categorySection;
                categorySection.category = category;
                mSections.push_back(std::move(categorySection));
            }
        }
    }

    if (!sourceModel()) {
        return;
    }

    for (auto row = 0, until = sourceModel()->rowCount(); row < until; ++row) {
        const QPersistentModelIndex index = sourceModel()->index(row, 0);
        mSections.at(sectionId(index)).rooms.push_back(index);
    }

    for (auto &section : mSections) {
        std::sort(section.rooms.begin(), section.rooms.end());
    }
}

auto RoomListHeadingsProxyModel::type(const QModelIndex &index) -> IndexType
{
    if (!index.isValid()) {
        return IndexType::Root;
    }

    if (index.internalId() == sectionMarker) {
        return IndexType::Section;
    }

    return IndexType::Channel;
}

#include "moc_roomlistheadingsproxymodel.cpp"
