/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "custombasemodel.h"

CustomBaseModel::CustomBaseModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CustomBaseModel::~CustomBaseModel() = default;

bool CustomBaseModel::loadMoreInProgress() const
{
    return mLoadMoreInProgress;
}

void CustomBaseModel::setLoadMoreInProgress(bool inProgress)
{
    if (mLoadMoreInProgress != inProgress) {
        mLoadMoreInProgress = inProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

QList<int> CustomBaseModel::excludeResizeToContentColumns() const
{
    return {};
}

void CustomBaseModel::insertElement(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // Nothing
}

void CustomBaseModel::removeElement(const QString &identifier)
{
    Q_UNUSED(identifier)
}

void CustomBaseModel::updateElement(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // Nothing
}

void CustomBaseModel::initialize()
{
    mLoadMoreInProgress = false;
    setHasFullList(false);
}

void CustomBaseModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool CustomBaseModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_custombasemodel.cpp"
