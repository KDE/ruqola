/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditwidget.h"
#include "misc/rolescombobox.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
PermissionsEditWidget::PermissionsEditWidget(QWidget *parent)
    : QWidget(parent)
    , mRolesCombobox(new RolesComboBox(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    auto label = new QLabel(i18n("Roles:"), this);
    label->setObjectName(QStringLiteral("label"));
    mRolesCombobox->setObjectName(QStringLiteral("mRolesCombobox"));
    mainLayout->addRow(label, mRolesCombobox);
}

PermissionsEditWidget::~PermissionsEditWidget() = default;

void PermissionsEditWidget::setRolesInfo(const QVector<RoleInfo> &rolesinfo)
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
