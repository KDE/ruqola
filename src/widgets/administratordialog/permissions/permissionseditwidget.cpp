/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditwidget.h"
#include "misc/rolescombobox.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
PermissionsEditWidget::PermissionsEditWidget(QWidget *parent)
    : QWidget(parent)
    , mRolesCombobox(new RolesComboBox(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    auto label = new QLabel(i18n("Roles:"), this);
    label->setObjectName(QStringLiteral("label"));
    mRolesCombobox->setObjectName(QStringLiteral("mRolesCombobox"));
    mainLayout->addWidget(label);
    mainLayout->addWidget(mRolesCombobox, 1);
}

PermissionsEditWidget::~PermissionsEditWidget() = default;

void PermissionsEditWidget::setRolesInfo(const QList<RoleInfo> &rolesinfo)
{
    mRolesCombobox->setRolesInfo(rolesinfo);
}

void PermissionsEditWidget::setRoles(const QStringList &lst)
{
    mRolesCombobox->setRoles(lst);
}

QStringList PermissionsEditWidget::roles() const
{
    return mRolesCombobox->roles();
}

#include "moc_permissionseditwidget.cpp"
