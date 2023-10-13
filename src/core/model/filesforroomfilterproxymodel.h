/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class FilesForRoomModel;
class LIBRUQOLACORE_EXPORT FilesForRoomFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilesForRoomFilterProxyModel(FilesForRoomModel *fileModel = nullptr, QObject *parent = nullptr);
    ~FilesForRoomFilterProxyModel() override;

    void setFilterString(const QString &string);
    [[nodiscard]] int total() const;

    [[nodiscard]] bool hasFullList() const;

    [[nodiscard]] int attachmentCount() const;

    void setTypeGroup(const QString &typeGroup);
    void resetTypeGroup();

    void clear();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    QString mTypeGroup;
    FilesForRoomModel *const mFilesForRoomModel;
};
