/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directorybasemodel.h"
#include "discussions.h"

DirectoryBaseModel::DirectoryBaseModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

DirectoryBaseModel::~DirectoryBaseModel()
{
}

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

void DirectoryBaseModel::removeElement(const QString &identifier)
{
    Q_UNUSED(identifier);
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
