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
}

void RoomListHeadingsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (auto *oldModel = this->sourceModel()) {
        disconnect(oldModel, nullptr, this, nullptr);
    }

    QAbstractProxyModel::setSourceModel(sourceModel);

    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &RoomListHeadingsProxyModel::onRowsInserted);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &RoomListHeadingsProxyModel::onRowsAboutToBeRemoved);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, [this]{ Q_EMIT layoutAboutToBeChanged(); });
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, &RoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, [this]{ Q_EMIT layoutChanged(); });

    connect(sourceModel, &QAbstractItemModel::dataChanged, this, &RoomListHeadingsProxyModel::onDataChanged);

    connect(sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &RoomListHeadingsProxyModel::beginResetModel);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &RoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &RoomListHeadingsProxyModel::endResetModel);

    connect(sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &RoomListHeadingsProxyModel::layoutAboutToBeChanged);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &RoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &RoomListHeadingsProxyModel::layoutChanged);

    rebuildSections();

    endResetModel();
}

int RoomListHeadingsProxyModel::rowCount(const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
        return sectionCount;
    case IndexType::Section:
        return int(mSections.at(parent.row()).size());
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
        return createIndex(row, column, sectionCount);
    case IndexType::Section:
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
        return createIndex(int(child.internalId()), 0, sectionCount);
    }
    Q_UNREACHABLE();
}

QVariant RoomListHeadingsProxyModel::data(const QModelIndex &index, int role) const
{
    switch (type(index)) {
    case IndexType::Root:
        return {};
    case IndexType::Section:
        switch (role) {
        case Qt::ItemDataRole::DisplayRole:
            return RoomModel::sectionName(RoomModel::Section(index.row()));
        case Qt::BackgroundRole:
            return QApplication::palette().brush(QPalette::Window);
        default:
            return {};
        }
    case IndexType::Channel:
        return sourceModel()->data(mapToSource(index), role);
    }
    Q_UNREACHABLE();
}

QModelIndex RoomListHeadingsProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceModel())
        return {};

    if (!sourceIndex.isValid())
        return {};

    for (auto sectionId = size_t(0), iMax = mSections.size(); sectionId < iMax; ++sectionId) {
        const auto &section = mSections.at(sectionId);

        const auto it = std::lower_bound(section.cbegin(), section.cend(), sourceIndex);
        if (it != section.cend() && *it == sourceIndex)
            return createIndex(int(it - section.cbegin()), 0, sectionId);
    }

    return {};
}

QModelIndex RoomListHeadingsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!sourceModel())
        return {};

    switch (type(proxyIndex)) {
    case IndexType::Root:
    case IndexType::Section:
        return {};
    case IndexType::Channel:
        return mSections.at(proxyIndex.internalId()).at(proxyIndex.row());
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
        const auto newSectionId = int(index.data(RoomModel::RoomSection).value<RoomModel::Section>());
        auto &newSection = mSections.at(newSectionId);

        const auto newLocation = std::lower_bound(newSection.cbegin(), newSection.cend(), index);
        const auto newLocationRow = int(newLocation - newSection.cbegin());

        beginInsertRows(createIndex(newSectionId, 0, sectionCount), newLocationRow, newLocationRow);

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
        auto &oldSection = mSections.at(oldSectionId);

        beginRemoveRows(ourOldIndex.parent(), ourOldIndex.row(), ourOldIndex.row());

        oldSection.erase(oldSection.begin() + ourOldIndex.row());

        endRemoveRows();
    }
}

void RoomListHeadingsProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto proxyIndex = mapFromSource(sourceIndex);
        Q_EMIT dataChanged(proxyIndex, proxyIndex, roles);
    }

    if (!roles.empty() && !roles.contains(RoomModel::RoomSection))
        return;

    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto ourOldIndex = mapFromSource(sourceIndex);

        const auto oldSectionId = int(ourOldIndex.internalId());
        const auto newSectionId = int(sourceIndex.data(RoomModel::RoomSection).value<RoomModel::Section>());

        if (oldSectionId == newSectionId)
            continue;

        auto &oldSection = mSections.at(oldSectionId);
        auto &newSection = mSections.at(newSectionId);

        const auto newLocation = std::lower_bound(newSection.cbegin(), newSection.cend(), sourceIndex);
        const auto newLocationRow = int(newLocation - newSection.cbegin());

        beginMoveRows(ourOldIndex.parent(), ourOldIndex.row(), ourOldIndex.row(), createIndex(newSectionId, 0, sectionCount), newLocationRow);

        auto persistantIndex = oldSection[ourOldIndex.row()];
        oldSection.erase(oldSection.begin() + ourOldIndex.row());
        newSection.insert(newLocation, persistantIndex);

        endMoveRows();
    }
}

void RoomListHeadingsProxyModel::rebuildSections()
{
    for (auto &section : mSections)
        section.clear();

    for (auto row = 0, until = sourceModel()->rowCount(); row < until; ++row) {
        const QPersistentModelIndex index = sourceModel()->index(row, 0);
        const auto newSectionId = uint(index.data(RoomModel::RoomSection).value<RoomModel::Section>());
        auto &newSection = mSections.at(newSectionId);

        newSection.push_back(index);
    }

    for (auto &section : mSections)
        std::sort(section.begin(), section.end());
}

auto RoomListHeadingsProxyModel::type(const QModelIndex &index) const -> IndexType
{
    if (!index.isValid())
        return IndexType::Root;

    if (index.internalId() == sectionCount)
        return IndexType::Section;

    return IndexType::Channel;
}
