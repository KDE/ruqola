/*
   SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractItemModel>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AccountsChannelsModel : public QAbstractItemModel
{
public:
    explicit AccountsChannelsModel(QObject *parent = nullptr);
    ~AccountsChannelsModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QModelIndex modelRoot(QAbstractItemModel *model) const;
    QAbstractItemModel *rootModel(const QModelIndex &root) const;
    LIBRUQOLACORE_NO_EXPORT void mapModelToIndex(QAbstractItemModel *model, const std::function<QModelIndex()> &root);
    LIBRUQOLACORE_NO_EXPORT void unproxyModel(QAbstractItemModel *model);

    struct ProxyIndex {
        QAbstractItemModel *model = nullptr;
        std::function<QModelIndex()> root;
    };
    QVector<ProxyIndex> mProxied;
};
