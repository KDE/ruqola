/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "permissions/permission.h"
#include <QDialog>
class ExplorePermissionWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExplorePermissionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExplorePermissionDialog(QWidget *parent = nullptr);
    ~ExplorePermissionDialog() override;

    void setPermissions(const QVector<Permission> &permissions);

private:
    void readConfig();
    void writeConfig();
    ExplorePermissionWidget *const mExporePermissionWidget;
};
