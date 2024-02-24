/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class PermissionsEditWidget;
class RoleInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PermissionsEditDialog(QWidget *parent = nullptr, const QString &permission = {});
    ~PermissionsEditDialog() override;

    void setRoles(const QStringList &lst);
    [[nodiscard]] QStringList roles() const;

    void setRolesInfo(const QList<RoleInfo> &rolesinfo);

private:
    PermissionsEditWidget *const mPermissionsWidget;
};
