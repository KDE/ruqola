/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagefilterproxymodel.h"
#include "commonmessagesmodel.h"

CommonMessageFilterProxyModel::CommonMessageFilterProxyModel(CommonMessagesModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mCommonMessagesModel(model)
{
    setSourceModel(mCommonMessagesModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(CommonMessagesModel::Timestamp);
    sort(0, Qt::DescendingOrder);
    connect(mCommonMessagesModel, &CommonMessagesModel::stringNotFoundChanged, this, &CommonMessageFilterProxyModel::stringNotFoundChanged);
    connect(mCommonMessagesModel, &CommonMessagesModel::loadingInProgressChanged, this, &CommonMessageFilterProxyModel::loadingInProgressChanged);
}

CommonMessageFilterProxyModel::~CommonMessageFilterProxyModel() = default;

bool CommonMessageFilterProxyModel::loadCommonMessagesInProgress() const
{
    return mCommonMessagesModel->loadCommonMessagesInProgress();
}

void CommonMessageFilterProxyModel::clearModel()
{
    mCommonMessagesModel->clearModel();
}

void CommonMessageFilterProxyModel::setSearchText(const QString &str)
{
    mCommonMessagesModel->setSearchText(str);
}

#include "moc_commonmessagefilterproxymodel.cpp"
