/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include "roles/roleinfo.h"
#include <QComboBox>
class RolesModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RolesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit RolesComboBox(QWidget *parent = nullptr);
    ~RolesComboBox() override;

    void setRoles(const QStringList &lst);
    [[nodiscard]] QStringList roles() const;

    void setRolesInfo(const QVector<RoleInfo> &roleInfo);

private:
    RolesModel *const mRolesModel;
};
