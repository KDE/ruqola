/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>
class DiscussionsModel;
class LIBRUQOLACORE_EXPORT DiscussionsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DiscussionsFilterProxyModel(DiscussionsModel *model = nullptr, QObject *parent = nullptr);
    ~DiscussionsFilterProxyModel() override;

    void setFilterString(const QString &string);
    void clearFilter();
    [[nodiscard]] int total() const;

    [[nodiscard]] bool hasFullList() const;
    [[nodiscard]] bool loadMoreDiscussionsInProgress() const;

    void clear();

Q_SIGNALS:
    void hasFullListChanged();
    void loadingInProgressChanged();

private:
    DiscussionsModel *const mDiscussionsModel;
};
