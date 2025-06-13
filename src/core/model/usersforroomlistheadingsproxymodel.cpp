/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroomlistheadingsproxymodel.h"
#include <QApplication>
#include <QFont>
#include <QPalette>

UsersForRoomListHeadingsProxyModel::UsersForRoomListHeadingsProxyModel(QObject *parent)
    : QAbstractProxyModel{parent}
{
}

UsersForRoomListHeadingsProxyModel::~UsersForRoomListHeadingsProxyModel() = default;

QVariant UsersForRoomListHeadingsProxyModel::data(const QModelIndex &index, int role) const
{
    switch (type(index)) {
    case IndexType::Root:
        return {};
    case IndexType::Section:
        switch (role) {
        case Qt::DisplayRole: {
            const QString sectionName = UsersForRoomModel::sectionName(UsersForRoomModel::SectionStatus(index.row()));
            return sectionName;
        }
        case Qt::BackgroundRole:
            return QApplication::palette().brush(QPalette::Window);
        case Qt::FontRole: {
            auto font = qvariant_cast<QFont>(QAbstractProxyModel::data(index, Qt::FontRole));
            font.setBold(true);
            return font;
        }
        default:
            return {};
        }
    case IndexType::Status:
        return sourceModel()->data(mapToSource(index), role);
    }
    Q_UNREACHABLE();
    return {};
}

QModelIndex UsersForRoomListHeadingsProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
        return createIndex(row, column, sectionCount);
    case IndexType::Section:
        return createIndex(row, column, parent.row());
    case IndexType::Status:
        return {};
    }
    Q_UNREACHABLE();
    return {};
}

QModelIndex UsersForRoomListHeadingsProxyModel::parent(const QModelIndex &child) const
{
    switch (type(child)) {
    case IndexType::Root:
    case IndexType::Section:
        return {};
    case IndexType::Status:
        return createIndex(int(child.internalId()), 0, sectionCount);
    }
    Q_UNREACHABLE();
    return {};
}

int UsersForRoomListHeadingsProxyModel::rowCount(const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
        return sectionCount;
    case IndexType::Section:
        return int(mSections.at(parent.row()).size());
    case IndexType::Status:
        return 0;
    }
    Q_UNREACHABLE();
    return {};
}

int UsersForRoomListHeadingsProxyModel::columnCount(const QModelIndex &parent) const
{
    switch (type(parent)) {
    case IndexType::Root:
    case IndexType::Section:
        return 1;
    case IndexType::Status:
        return 0;
    }
    Q_UNREACHABLE();
    return {};
}

Qt::ItemFlags UsersForRoomListHeadingsProxyModel::flags(const QModelIndex &proxyIndex) const
{
    switch (type(proxyIndex)) {
    case IndexType::Root:
        return {};
    case IndexType::Section:
        return Qt::ItemFlag::ItemIsEnabled;
    case IndexType::Status:
        return QAbstractProxyModel::flags(proxyIndex);
    }
    Q_UNREACHABLE();
    return {};
}

bool UsersForRoomListHeadingsProxyModel::hasChildren(const QModelIndex &index) const
{
    // Don't use the QAbstractProxyModel override which fails on non-proxied indices
    return QAbstractItemModel::hasChildren(index); // NOLINT(bugprone-parent-virtual-call)
}

void UsersForRoomListHeadingsProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (auto *oldModel = this->sourceModel()) {
        disconnect(oldModel, nullptr, this, nullptr);
    }

    QAbstractProxyModel::setSourceModel(sourceModel);

    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &UsersForRoomListHeadingsProxyModel::onRowsInserted);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &UsersForRoomListHeadingsProxyModel::onRowsAboutToBeRemoved);

    connect(sourceModel, &QAbstractItemModel::rowsAboutToBeMoved, this, [this] {
        Q_EMIT layoutAboutToBeChanged();
    });
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, &UsersForRoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::rowsMoved, this, [this] {
        Q_EMIT layoutChanged();
    });

    connect(sourceModel, &QAbstractItemModel::dataChanged, this, &UsersForRoomListHeadingsProxyModel::onDataChanged);

    connect(sourceModel, &QAbstractItemModel::modelAboutToBeReset, this, &UsersForRoomListHeadingsProxyModel::beginResetModel);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &UsersForRoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &UsersForRoomListHeadingsProxyModel::endResetModel);

    connect(sourceModel, &QAbstractItemModel::layoutAboutToBeChanged, this, &UsersForRoomListHeadingsProxyModel::layoutAboutToBeChanged);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &UsersForRoomListHeadingsProxyModel::rebuildSections);
    connect(sourceModel, &QAbstractItemModel::layoutChanged, this, &UsersForRoomListHeadingsProxyModel::layoutChanged);

    rebuildSections();

    endResetModel();
}

QModelIndex UsersForRoomListHeadingsProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!sourceModel()) {
        return {};
    }

    switch (type(proxyIndex)) {
    case IndexType::Root:
    case IndexType::Section:
        return {};
    case IndexType::Status:
        return mSections.at(proxyIndex.internalId()).at(proxyIndex.row());
    }
    Q_UNREACHABLE();
    return {};
}

QModelIndex UsersForRoomListHeadingsProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceModel()) {
        return {};
    }

    if (!sourceIndex.isValid()) {
        return {};
    }

    for (auto sectionId = size_t(0), iMax = mSections.size(); sectionId < iMax; ++sectionId) {
        const auto &section = mSections.at(sectionId);

        const auto it = std::lower_bound(section.cbegin(), section.cend(), sourceIndex);
        if (it != section.cend() && *it == sourceIndex)
            return createIndex(int(it - section.cbegin()), 0, sectionId);
    }

    return {};
}

void UsersForRoomListHeadingsProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        const QPersistentModelIndex index = sourceModel()->index(row, 0, parent);
        const auto newSectionId = int(index.data(UsersForRoomModel::Section).value<UsersForRoomModel::SectionStatus>());
        auto &newSection = mSections.at(newSectionId);

        const auto newLocation = std::lower_bound(newSection.cbegin(), newSection.cend(), index);
        const auto newLocationRow = int(newLocation - newSection.cbegin());

        beginInsertRows(createIndex(newSectionId, 0, sectionCount), newLocationRow, newLocationRow);

        newSection.insert(newLocation, index);

        endInsertRows();
    }
}

void UsersForRoomListHeadingsProxyModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
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

void UsersForRoomListHeadingsProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto proxyIndex = mapFromSource(sourceIndex);
        Q_EMIT dataChanged(proxyIndex, proxyIndex, roles);
    }

    if (!roles.empty() && !roles.contains(UsersForRoomModel::Section)) {
        return;
    }
    for (auto row = topLeft.row(), last = bottomRight.row(); row <= last; ++row) {
        const auto sourceIndex = topLeft.siblingAtRow(row);
        const auto ourOldIndex = mapFromSource(sourceIndex);

        const auto oldSectionId = int(ourOldIndex.internalId());
        const auto newSectionId = int(sourceIndex.data(UsersForRoomModel::Section).value<UsersForRoomModel::SectionStatus>());

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

void UsersForRoomListHeadingsProxyModel::rebuildSections()
{
    for (auto &section : mSections) {
        section.clear();
    }

    for (auto row = 0, until = sourceModel()->rowCount(); row < until; ++row) {
        const QPersistentModelIndex index = sourceModel()->index(row, 0);
        const auto newSectionId = uint(index.data(UsersForRoomModel::Section).value<UsersForRoomModel::SectionStatus>());
        auto &newSection = mSections.at(newSectionId);

        newSection.push_back(index);
    }

    for (auto &section : mSections) {
        std::sort(section.begin(), section.end());
    }
}

auto UsersForRoomListHeadingsProxyModel::type(const QModelIndex &index) const -> IndexType
{
    if (!index.isValid()) {
        return IndexType::Root;
    }

    if (index.internalId() == sectionCount) {
        return IndexType::Section;
    }

    return IndexType::Status;
}
