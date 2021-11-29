/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "roles/roleinfo.h"
#include <QDialog>
class PermissionsEditWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PermissionsEditDialog(QWidget *parent = nullptr, const QString &permission = {});
    ~PermissionsEditDialog() override;

    void setRoles(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList roles() const;

    void setRolesInfo(const QVector<RoleInfo> &rolesinfo);

private:
    PermissionsEditWidget *const mPermissionsWidget;
};
