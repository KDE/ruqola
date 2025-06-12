/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "usersforroommodel.h"
#include <QAbstractProxyModel>
class UsersForRoomListHeadingsProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit UsersForRoomListHeadingsProxyModel(QObject *parent = nullptr);
    ~UsersForRoomListHeadingsProxyModel() override;

    static constexpr uint sectionCount = uint(UsersForRoomModel::SectionStatus::NSections);

    // QAbstractItemModel interface
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    [[nodiscard]] bool hasChildren(const QModelIndex &index) const override;

    // QAbstractProxyModel interface
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    [[nodiscard]] QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    [[nodiscard]] QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

private:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    void rebuildSections();

    // each section vector is kept sorted for performance reasons
    std::array<std::vector<QPersistentModelIndex>, sectionCount> mSections;

    enum class IndexType : uint8_t {
        Root,
        Section,
        Status,
    };
    [[nodiscard]] IndexType type(const QModelIndex &index) const;
};
