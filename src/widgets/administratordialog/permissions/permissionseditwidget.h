/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "roles/roleinfo.h"
#include <QWidget>
class RolesComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PermissionsEditWidget(QWidget *parent = nullptr);
    ~PermissionsEditWidget() override;
    void setRoles(const QStringList &lst);
    Q_REQUIRED_RESULT QStringList roles() const;

    void setRolesInfo(const QVector<RoleInfo> &rolesinfo);

private:
    RolesComboBox *const mRolesCombobox;
};
