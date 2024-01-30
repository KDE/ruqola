/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "roles/roleinfo.h"
#include <QWidget>
class PermissionsTreeView;
class QLineEdit;
class PermissionsModel;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPermissionListAllDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPermissionUpdateDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotEditRoles(const QStringList &roles, const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyDoubleClickRoles(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void modifyRoles(const QModelIndex &index);
    QVector<RoleInfo> mRoleInfo;
    PermissionsTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    PermissionsModel *const mAdminPermissionsModel;
    QSortFilterProxyModel *const mPermissionFilterProxyModel;
    RocketChatAccount *const mRocketChatAccount;
};
