/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include "utils.h"
#include <QAbstractListModel>
#include <QList>
class UsersModel;
class LIBRUQOLACORE_EXPORT UsersForRoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UsersForRoomRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        Name,
        IconStatus,
        DisplayName,
        StatusStr,
        Status,
        AvatarInfo,
        Roles,
        Section,
    };
    Q_ENUM(UsersForRoomRoles)

    enum class SectionStatus : uint8_t {
        Owner,
        Online,
        Busy,
        Away,
        Offline,
        Unknown,
        NSections,
    };
    Q_ENUM(SectionStatus)

    explicit UsersForRoomModel(QObject *parent = nullptr);
    ~UsersForRoomModel() override;

    void setUsers(const QList<User> &users);

    void clear();
    int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void parseUsersForRooms(const QJsonObject &root, UsersModel *model, bool restapi);
    void setUserStatusChanged(const User &newuser);

    [[nodiscard]] int total() const;
    void setTotal(int total);

    [[nodiscard]] int offset() const;
    void setOffset(int offset);

    [[nodiscard]] bool hasFullList() const;
    void setHasFullList(bool hasFullList);
    [[nodiscard]] int usersCount() const;

    void setLoadMoreUsersInProgress(bool inProgress);
    [[nodiscard]] bool loadMoreUsersInProgress() const;
    [[nodiscard]] static QString sectionName(UsersForRoomModel::SectionStatus sectionId);
Q_SIGNALS:
    void hasFullListChanged();
    void userStatusChanged(const QByteArray &userId);
    void loadingInProgressChanged();

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Utils::AvatarInfo avatarInfo(const User &user) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateDisplayName(const User &user) const;
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT UsersForRoomModel::SectionStatus section(const User &user) const;
    QList<User> mUsers;
    int mTotal = 0;
    int mOffset = 0;
    bool mHasFullList = false;
    bool mLoadingInProgress = false;
};
