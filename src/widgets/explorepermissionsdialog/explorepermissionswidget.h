/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "permissions/permission.h"
#include <QWidget>
class QLineEdit;
class PermissionsModel;
class QSortFilterProxyModel;
class QTreeView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExplorePermissionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExplorePermissionsWidget(QWidget *parent = nullptr);
    ~ExplorePermissionsWidget() override;

    void setPermissions(const QVector<Permission> &permissions);

private:
    void slotFilterTextChanged(const QString &str);
    QTreeView *const mTreeView;
    QLineEdit *const mSearchLineWidget;
    PermissionsModel *const mAdminPermissionsModel;
    QSortFilterProxyModel *const mPermissionFilterProxyModel;
};
