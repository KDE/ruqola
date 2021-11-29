/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "discussionsfilterproxymodel.h"
#include "discussionsmodel.h"
DiscussionsFilterProxyModel::DiscussionsFilterProxyModel(DiscussionsModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mDiscussionsModel(model)
{
    setSourceModel(mDiscussionsModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(DiscussionsModel::Description);
    setSortRole(DiscussionsModel::SortByTimeStamp);
    sort(0, Qt::DescendingOrder);
    connect(mDiscussionsModel, &DiscussionsModel::hasFullListChanged, this, &DiscussionsFilterProxyModel::hasFullListChanged);
    connect(mDiscussionsModel, &DiscussionsModel::loadingInProgressChanged, this, &DiscussionsFilterProxyModel::loadingInProgressChanged);
}

DiscussionsFilterProxyModel::~DiscussionsFilterProxyModel() = default;

void DiscussionsFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

void DiscussionsFilterProxyModel::clearFilter()
{
    setFilterFixedString(QString());
}

int DiscussionsFilterProxyModel::total() const
{
    return mDiscussionsModel->total();
}

bool DiscussionsFilterProxyModel::hasFullList() const
{
    return mDiscussionsModel->hasFullList();
}

bool DiscussionsFilterProxyModel::loadMoreDiscussionsInProgress() const
{
    return mDiscussionsModel->loadMoreDiscussionsInProgress();
}
