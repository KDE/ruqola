/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "permissions/permission.h"
#include <QDialog>
class ExplorePermissionsWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExplorePermissionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExplorePermissionsDialog(QWidget *parent = nullptr);
    ~ExplorePermissionsDialog() override;

    void setPermissions(const QVector<Permission> &permissions);
    void setOWnRoles(const QStringList &roleStr);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ExplorePermissionsWidget *const mExporePermissionWidget;
};
