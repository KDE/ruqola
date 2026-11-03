/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include "roommodel.h"
#include <QAbstractProxyModel>
#include <QPointer>

#include <array>
#include <limits>
#include <vector>

/*!
 * Groups the rooms of a RoomModel in sections: the standard sections, plus one section per category
 * of the account, inserted after the favorites. A room which belongs to a category is only listed in
 * that category, unless it is in a dynamic section (unread, incoming calls, live chats).
 */
class LIBRUQOLACORE_EXPORT RoomListHeadingsProxyModel final : public QAbstractProxyModel
{
    Q_OBJECT

public:
    explicit RoomListHeadingsProxyModel(QObject *parent = nullptr);

    /*! The number of standard sections. When the account has categories, the category sections come
     *  in addition, so a standard section is not always at the row of its RoomModel::Section. */
    static constexpr uint sectionCount = uint(RoomModel::Section::NSections);

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
    LIBRUQOLACORE_NO_EXPORT void onRowsInserted(const QModelIndex &parent, int first, int last);
    LIBRUQOLACORE_NO_EXPORT void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    LIBRUQOLACORE_NO_EXPORT void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    LIBRUQOLACORE_NO_EXPORT void rebuildSections();
    LIBRUQOLACORE_NO_EXPORT void resetSections();

    struct Section {
        //! The category this section stands for, invalid when it is a standard section.
        Category category;
        //! Only meaningful for a standard section.
        RoomModel::Section kind = RoomModel::Section::NSections;
        // kept sorted for performance reasons
        std::vector<QPersistentModelIndex> rooms;
    };

    //! The standard sections, with the category sections after the favorites.
    std::vector<Section> mSections;
    //! The row in mSections of each standard section.
    std::array<int, sectionCount> mStandardSectionRows{};
    QPointer<RoomModel> mRoomModel;

    //! Stored in the internal id of a section index, as the number of sections isn't constant.
    static constexpr quintptr sectionMarker = std::numeric_limits<quintptr>::max();

    enum class IndexType : uint8_t {
        Root,
        Section,
        Channel,
    };
    [[nodiscard]] static LIBRUQOLACORE_NO_EXPORT IndexType type(const QModelIndex &index);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool isValidSectionRow(int row) const;
    //! Returns the row of the section the given source index belongs to.
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT int sectionId(const QModelIndex &sourceIndex) const;
};
