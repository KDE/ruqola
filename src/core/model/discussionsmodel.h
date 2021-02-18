/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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
class Discussions;
class LIBRUQOLACORE_EXPORT DiscussionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DiscussionRoles { ParentId = Qt::UserRole + 1, Description, NumberOfMessages, LastMessage, DiscussionRoomId, TimeStamp, SortByTimeStamp };
    Q_ENUM(DiscussionRoles)

    explicit DiscussionsModel(QObject *parent = nullptr);
    ~DiscussionsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void setDiscussions(const Discussions &discussions);

    void initialize();

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    void parseDiscussions(const QJsonObject &discussionsObj, const QString &roomId);

    void addMoreDiscussions(const QJsonObject &discussionsObj);
    Discussions *discussions() const;
    Q_REQUIRED_RESULT int total() const;

    void setHasFullList(bool state);
    Q_REQUIRED_RESULT bool hasFullList() const;

    Q_REQUIRED_RESULT bool loadMoreDiscussionsInProgress() const;
    void setLoadMoreDiscussionsInProgress(bool loadMoreDiscussionsInProgress);

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    Q_DISABLE_COPY(DiscussionsModel)
    void checkFullList();
    QString mRoomId;
    bool mLoadMoreDiscussionsInProgress = false;
    bool mHasFullList = false;
    Discussions *const mDiscussions;
};

