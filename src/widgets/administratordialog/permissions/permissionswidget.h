/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "roles/roleinfo.h"
#include <QWidget>
class PermissionsTreeView;
class QLineEdit;
class AdminPermissionsModel;
class QSortFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PermissionsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PermissionsWidget() override;
    void initialize();

private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotPermissionListAllDone(const QJsonObject &obj);
    void slotPermissionUpdateDone(const QJsonObject &obj);
    void slotEditRoles(const QStringList &roles, const QString &identifier);
    void slotFilterTextChanged(const QString &str);
    void slotModifyDoubleClickRoles(const QModelIndex &index);
    void modifyRoles(const QModelIndex &index);
    QVector<RoleInfo> mRoleInfo;
    PermissionsTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    AdminPermissionsModel *const mAdminPermissionsModel;
    QSortFilterProxyModel *const mPermissionFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
};
