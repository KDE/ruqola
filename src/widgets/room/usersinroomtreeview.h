/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QTreeView>
class UsersForRoomListHeadingsProxyModel;
class UsersForRoomFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit UsersInRoomTreeView(QWidget *parent = nullptr);
    ~UsersInRoomTreeView() override;

    [[nodiscard]] UsersForRoomFilterProxyModel *usersForRoomFilterProxy() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    [[nodiscard]] bool event(QEvent *ev) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void generalPaletteChanged();
    QColor mTextColor;
    UsersForRoomListHeadingsProxyModel *const mUsersForRoomListHeadingsProxyModel;
    UsersForRoomFilterProxyModel *const mUsersForRoomFilterProxy;
};
