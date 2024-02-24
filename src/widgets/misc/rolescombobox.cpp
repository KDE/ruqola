/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "rolescombobox.h"
#include "model/rolesmodel.h"
#include "roles/roleinfo.h"

RolesComboBox::RolesComboBox(QWidget *parent)
    : QComboBox(parent)
    , mRolesModel(new RolesModel(this))
{
    setModel(mRolesModel);
}

RolesComboBox::~RolesComboBox() = default;

void RolesComboBox::setRolesInfo(const QList<RoleInfo> &roleInfo)
{
    mRolesModel->setRoles(roleInfo);
}

void RolesComboBox::setRoles(const QStringList &lst)
{
    mRolesModel->setRolesSelected(lst);
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

QStringList RolesComboBox::roles() const
{
    return mRolesModel->rolesSelected();
}

#include "moc_rolescombobox.cpp"
