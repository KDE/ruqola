/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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

private:
    void readConfig();
    void writeConfig();
    ExplorePermissionsWidget *const mExporePermissionWidget;
};
