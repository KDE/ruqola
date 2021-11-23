/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roleeditdialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

RoleEditDialog::RoleEditDialog(QWidget *parent)
    : QDialog(parent)
    , mRoleEditWidget(new RoleEditWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Role"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mRoleEditWidget->setObjectName(QStringLiteral("mRoleEditWidget"));
    mainLayout->addWidget(mRoleEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    connect(button, &QDialogButtonBox::rejected, this, &RoleEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &RoleEditDialog::accept);
    resize(350, 50);
    connect(mRoleEditWidget, &RoleEditWidget::updateOkButton, this, [okButton](bool b) {
        okButton->setEnabled(b);
    });
}

RoleEditDialog::~RoleEditDialog() = default;

void RoleEditDialog::setRoleEditDialogInfo(const RoleEditWidget::RoleEditDialogInfo &info)
{
    setWindowTitle(i18nc("@title:window", "Edit Role"));
    mRoleEditWidget->setRoleEditDialogInfo(info);
}

RoleEditWidget::RoleEditDialogInfo RoleEditDialog::roleEditDialogInfo() const
{
    return mRoleEditWidget->roleEditDialogInfo();
}
