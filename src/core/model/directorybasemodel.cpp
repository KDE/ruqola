/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorybasemodel.h"
#include "discussions/discussions.h"

DirectoryBaseModel::DirectoryBaseModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

DirectoryBaseModel::~DirectoryBaseModel() = default;

bool DirectoryBaseModel::loadMoreInProgress() const
{
    return mLoadMoreInProgress;
}

void DirectoryBaseModel::setLoadMoreInProgress(bool inProgress)
{
    if (mLoadMoreInProgress != inProgress) {
        mLoadMoreInProgress = inProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

QList<int> DirectoryBaseModel::excludeResizeToContentColumns() const
{
    return {};
}

void DirectoryBaseModel::insertElement(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // Nothing
}

void DirectoryBaseModel::removeElement(const QString &identifier)
{
    Q_UNUSED(identifier)
}

void DirectoryBaseModel::updateElement(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // Nothing
}

void DirectoryBaseModel::initialize()
{
    mLoadMoreInProgress = false;
    setHasFullList(false);
}

void DirectoryBaseModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool DirectoryBaseModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_directorybasemodel.cpp"
