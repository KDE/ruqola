/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
    void slotFilterTextChanged(const QString &str);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void addRole();
    void addUserInRole(const QModelIndex &modelIndex);
    void modifyRole(const QModelIndex &modelIndex);
    void deleteRole(const QModelIndex &modelIndex);
    void slotModifyDoubleClickRoles(const QModelIndex &index);
    void slotRoleCreateDone();
    void slotRoleUpdateDone();
    void slotRoleDeleteDone();
    RolesTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    AdminRolesModel *const mAdminRolesModel;
    QSortFilterProxyModel *const mRoleFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
};
