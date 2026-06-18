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
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowBannedUsersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowBannedUsersWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowBannedUsersWidget() override;

    void setRoomId(const QByteArray &roomId);

Q_SIGNALS:
    void loadMoreBannedUsers();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessageTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    QLineEdit *const mSearchBannedUserLineEdit;
    QLabel *const mInfo;
    QListView *const mListBannedUsers;
    BannedUsersModel *const mModel;
    BannedUsersFilterProxyModel *const mBannedUsersFilterProxyModel;
};
