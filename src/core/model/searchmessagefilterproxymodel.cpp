/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagefilterproxymodel.h"
#include "searchmessagemodel.h"

SearchMessageFilterProxyModel::SearchMessageFilterProxyModel(SearchMessageModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mSearchMessageModel(model)
{
    setSourceModel(mSearchMessageModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(SearchMessageModel::Timestamp);
    sort(0, Qt::DescendingOrder);
    connect(mSearchMessageModel, &SearchMessageModel::stringNotFoundChanged, this, &SearchMessageFilterProxyModel::stringNotFoundChanged);
    connect(mSearchMessageModel, &SearchMessageModel::loadingInProgressChanged, this, &SearchMessageFilterProxyModel::loadingInProgressChanged);
}

SearchMessageFilterProxyModel::~SearchMessageFilterProxyModel() = default;

bool SearchMessageFilterProxyModel::stringNotFound() const
{
    return mSearchMessageModel->stringNotFound();
}

bool SearchMessageFilterProxyModel::loadSearchMessageInProgress() const
{
    return mSearchMessageModel->loadSearchMessageInProgress();
}
