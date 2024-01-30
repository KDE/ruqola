/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "customusers/customuserstatus.h"
#include <QTreeWidget>

#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class CustomUserStatusTreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit CustomUserStatusTreeWidgetItem(QTreeWidget *parent = nullptr);
    ~CustomUserStatusTreeWidgetItem() override;

    [[nodiscard]] CustomUserStatus userStatus() const;
    void setUserStatus(const CustomUserStatus &userStatus);

private:
    CustomUserStatus mUserStatus;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT CustomUserStatusTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum UserStatusColumn {
        Name = 0,
        Presence,
        Identifier,
    };
    explicit CustomUserStatusTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CustomUserStatusTreeWidget() override;
    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void addClicked();
    LIBRUQOLAWIDGETS_NO_EXPORT void editClicked();
    LIBRUQOLAWIDGETS_NO_EXPORT void removeClicked();
    RocketChatAccount *const mRocketChatAccount;
};
