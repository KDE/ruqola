/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RolesComboBox;
class RoleInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PermissionsEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PermissionsEditWidget(QWidget *parent = nullptr);
    ~PermissionsEditWidget() override;
    void setRoles(const QStringList &lst);
    [[nodiscard]] QStringList roles() const;

    void setRolesInfo(const QList<RoleInfo> &rolesinfo);

private:
    RolesComboBox *const mRolesCombobox;
};
