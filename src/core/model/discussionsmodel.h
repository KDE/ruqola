/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>
class Discussions;
class LIBRUQOLACORE_EXPORT DiscussionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DiscussionRoles {
        ParentId = Qt::UserRole + 1,
        Description,
        NumberOfMessages,
        LastMessage,
        DiscussionRoomId,
        TimeStamp,
        SortByTimeStamp,
        UserName,
    };
    Q_ENUM(DiscussionRoles)

    explicit DiscussionsModel(QObject *parent = nullptr);
    ~DiscussionsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void setDiscussions(const Discussions &discussions);

    void initialize();

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &roomId);

    void parseDiscussions(const QJsonObject &discussionsObj, const QString &roomId);

    void addMoreDiscussions(const QJsonObject &discussionsObj);
    Discussions *discussions() const;
    [[nodiscard]] int total() const;

    void setHasFullList(bool state);
    [[nodiscard]] bool hasFullList() const;

    [[nodiscard]] bool loadMoreDiscussionsInProgress() const;
    void setLoadMoreDiscussionsInProgress(bool loadMoreDiscussionsInProgress);

    void clear();

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    QString mRoomId;
    bool mLoadMoreDiscussionsInProgress = false;
    bool mHasFullList = false;
    Discussions *const mDiscussions;
};
