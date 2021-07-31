#pragma once

#include "libruqolacore_export.h"
#include "roommodel.h"
#include <QIdentityProxyModel>

#include <array>

class LIBRUQOLACORE_EXPORT RoomListHeadingsProxyModel final : public QIdentityProxyModel
{
    Q_OBJECT
public:
    explicit RoomListHeadingsProxyModel(QObject *parent = nullptr);

    enum Roles { IsHeading = 0x2711C5C2 };

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    Qt::ItemFlags flags(const QModelIndex &proxyIndex) const override;

private:
    void ensureCachedCounts() const;
    int numVisibleSections() const;
    RoomModel::Section proxyRowSection(int proxyRow) const;
    int proxyRowToSourceRow(int proxyRow) const;
    int sourceRowToProxyRow(int sourceRow) const;
    void setDirty();
    void dumpCache() const;
    friend class RoomListHeadingsProxyModelTest;

    mutable bool mDirty = true;
    // Number of rows in each section
    mutable std::array<int, int(RoomModel::Section::NSections)> mSectionCounts;
};
