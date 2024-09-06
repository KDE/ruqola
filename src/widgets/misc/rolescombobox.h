/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>
class RolesModel;
class RoleInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RolesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit RolesComboBox(QWidget *parent = nullptr);
    ~RolesComboBox() override;

    void setRoles(const QStringList &lst);
    [[nodiscard]] QStringList roles() const;

    void setRolesInfo(const QList<RoleInfo> &roleInfo);

Q_SIGNALS:
    void rolesChanged();

private:
    RolesModel *const mRolesModel;
};
