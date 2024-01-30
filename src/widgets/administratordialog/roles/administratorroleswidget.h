/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RolesTreeView;
class QLineEdit;
class AdminRolesModel;
class QSortFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRolesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorRolesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorRolesWidget() override;

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void addRole();
    LIBRUQOLAWIDGETS_NO_EXPORT void addUserInRole(const QModelIndex &modelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void modifyRole(const QModelIndex &modelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void deleteRole(const QModelIndex &modelIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyDoubleClickRoles(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoleCreateDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoleUpdateDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoleDeleteDone();
    RolesTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    AdminRolesModel *const mAdminRolesModel;
    QSortFilterProxyModel *const mRoleFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
};
