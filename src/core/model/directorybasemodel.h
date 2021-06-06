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

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>
class LIBRUQOLACORE_EXPORT DirectoryBaseModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DirectoryBaseModel(QObject *parent = nullptr);
    ~DirectoryBaseModel() override;

    Q_REQUIRED_RESULT virtual int total() const = 0;

    void setHasFullList(bool state);
    Q_REQUIRED_RESULT bool hasFullList() const;

    Q_REQUIRED_RESULT bool loadMoreInProgress() const;
    void setLoadMoreInProgress(bool loadMoreInProgress);

    virtual void parseElements(const QJsonObject &obj) = 0;

    virtual void addMoreElements(const QJsonObject &obj) = 0;

    virtual QList<int> hideColumns() const = 0;

    void initialize();
Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    Q_DISABLE_COPY(DirectoryBaseModel)
    bool mLoadMoreInProgress = false;
    bool mHasFullList = false;
    int mTotal = -1;
};
