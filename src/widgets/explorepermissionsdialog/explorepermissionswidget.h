/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "permissions/permission.h"
#include <QWidget>
class QLineEdit;
class PermissionsModel;
class ExplorePermissionsWidgetFilterProxyModel;
class QTreeView;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExplorePermissionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExplorePermissionsWidget(QWidget *parent = nullptr);
    ~ExplorePermissionsWidget() override;

    void setPermissions(const QVector<Permission> &permissions);

    void setOWnRoles(const QStringList &roleStr);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterTextChanged(const QString &str);
    QTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    PermissionsModel *const mAdminPermissionsModel;
    ExplorePermissionsWidgetFilterProxyModel *const mPermissionFilterProxyModel;
    QLabel *const mOwnRoles;
};
