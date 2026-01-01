/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ApplicationsPermissionWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ApplicationsPermissionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApplicationsPermissionDialog(QWidget *parent = nullptr);
    ~ApplicationsPermissionDialog() override;
    void setApplicationPermission(const QString &desc);

private:
    ApplicationsPermissionWidget *const mApplicationsPermissionWidget;
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
};
