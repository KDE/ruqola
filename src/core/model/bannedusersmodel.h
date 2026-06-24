/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "bannedusers/banneduser.h"
#include "bannedusers/bannedusers.h"
#include "libruqolacore_export.h"
#include "utils.h"
#include <QAbstractListModel>
class LIBRUQOLACORE_EXPORT BannedUsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum BannedUserRoles {
        Name = Qt::UserRole + 1,
        UserName,
        Identifier,
        AvatarInfo,
    };
    Q_ENUM(BannedUserRoles)

    explicit BannedUsersModel(QObject *parent = nullptr);
    ~BannedUsersModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void setBannedUsers(const QList<BannedUser> &files);

    [[nodiscard]] BannedUsers *bannedUsers() const;

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &roomId);

    void parseBannedUsers(const QJsonObject &bannedUsersObj, const QByteArray &roomId);
    void addMoreBannedUsers(const QJsonObject &bannedUsersObj);
    void initialize();
    [[nodiscard]] int total() const;

    [[nodiscard]] bool hasFullList() const;
    void setHasFullList(bool state);

    [[nodiscard]] bool loadMoreBannedUsersInProgress() const;
    void setLoadMoreBannedUsersInProgress(bool loadMoreFilesInProgress);

    void clear();
    void removeBannedUsers(const QString &userName);

Q_SIGNALS:
    void hasFullListChanged();
    void totalChanged();
    void loadingInProgressChanged();

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Utils::AvatarInfo avatarInfo(const BannedUser &user) const;
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    QByteArray mRoomId;
    bool mHasFullList = false;
    bool mLoadMoreBannedUsersInProgress = false;
    BannedUsers *const mBannedUsers;
};
