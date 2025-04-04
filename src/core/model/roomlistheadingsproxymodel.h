/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "roommodel.h"
#include <QAbstractProxyModel>

#include <array>
#include <vector>

class LIBRUQOLACORE_EXPORT RoomListHeadingsProxyModel final : public QAbstractProxyModel
{
    Q_OBJECT

public:
    explicit RoomListHeadingsProxyModel(QObject *parent = nullptr);

    static constexpr uint sectionCount = uint(RoomModel::Section::NSections);

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool hasChildren(const QModelIndex &index) const override;

    // QAbstractProxyModel interface
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

private:
    LIBRUQOLACORE_NO_EXPORT void onRowsInserted(const QModelIndex &parent, int first, int last);
    LIBRUQOLACORE_NO_EXPORT void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    LIBRUQOLACORE_NO_EXPORT void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    LIBRUQOLACORE_NO_EXPORT void rebuildSections();

    // each section vector is kept sorted for performance reasons
    std::array<std::vector<QPersistentModelIndex>, sectionCount> mSections;

    enum class IndexType : uint8_t {
        Root,
        Section,
        Channel,
    };
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT IndexType type(const QModelIndex &index) const;
};
