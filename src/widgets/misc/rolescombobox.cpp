/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "rolescombobox.h"
#include "connection.h"
#include "model/rolesmodel.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

RolesComboBox::RolesComboBox(QWidget *parent)
    : QComboBox(parent)
    , mRolesModel(new RolesModel(this))
{
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setModel(mRolesModel);
}

RolesComboBox::~RolesComboBox() = default;

void RolesComboBox::setRolesInfo(const QVector<RoleInfo> &roleInfo)
{
    mRolesModel->setRoles(roleInfo);
}

void RolesComboBox::setRoles(const QStringList &lst)
{
    mRolesModel->setRolesSelected(lst);
}

QStringList RolesComboBox::roles() const
{
    return mRolesModel->rolesSelected();
}
