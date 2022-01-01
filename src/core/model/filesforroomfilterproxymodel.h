/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT int total() const;

    Q_REQUIRED_RESULT bool hasFullList() const;

    Q_REQUIRED_RESULT int attachmentCount() const;

    void setTypeGroup(const QString &typeGroup);
    void resetTypeGroup();

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

