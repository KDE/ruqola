/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_roleeditdialog.cpp"
