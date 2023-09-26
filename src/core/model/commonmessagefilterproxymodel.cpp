/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagefilterproxymodel.h"
#include "commonmessagesmodel.h"

CommonMessageFilterProxyModel::CommonMessageFilterProxyModel(CommonMessagesModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mSearchMessageModel(model)
{
    setSourceModel(mSearchMessageModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(CommonMessagesModel::Timestamp);
    sort(0, Qt::DescendingOrder);
    connect(mSearchMessageModel, &CommonMessagesModel::stringNotFoundChanged, this, &CommonMessageFilterProxyModel::stringNotFoundChanged);
    connect(mSearchMessageModel, &CommonMessagesModel::loadingInProgressChanged, this, &CommonMessageFilterProxyModel::loadingInProgressChanged);
}

CommonMessageFilterProxyModel::~CommonMessageFilterProxyModel() = default;

bool CommonMessageFilterProxyModel::loadSearchMessageInProgress() const
{
    return mSearchMessageModel->loadSearchMessageInProgress();
}

void CommonMessageFilterProxyModel::clearModel()
{
    mSearchMessageModel->clearModel();
}

void CommonMessageFilterProxyModel::setSearchText(const QString &str)
{
    mSearchMessageModel->setSearchText(str);
}

#include "moc_commonmessagefilterproxymodel.cpp"
