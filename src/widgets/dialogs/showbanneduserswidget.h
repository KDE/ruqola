/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QLineEdit;
class QLabel;
class QListView;
class BannedUsersModel;
class RocketChatAccount;
class BannedUsersFilterProxyModel;
class ShowBannedUsersListView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowBannedUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowBannedUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowBannedUsersWidget() override;

    void setRoomId(const QByteArray &roomId);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchBannedUsersChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadBannedUsers();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotBannedUsersDone(const QJsonObject &obj, const QByteArray &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotBannedUsersFailed(const QString &serverErrorStr, const QString &descriptionError);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoadMoreBannedUsers();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUnbanUser(const QString &userName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUnBanUsersDone(const QString &userName);
    int mOffset = 0;
    RocketChatAccount *const mCurrentRocketChatAccount;
    QLineEdit *const mSearchBannedUserLineEdit;
    QLabel *const mInfo;
    ShowBannedUsersListView *const mListBannedUsers;
    BannedUsersModel *const mModel;
    BannedUsersFilterProxyModel *const mBannedUsersFilterProxyModel;
};
