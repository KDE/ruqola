/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoradduserdialog.h"
#include "administratoradduserwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

AdministratorAddUserDialog::AdministratorAddUserDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAdministratorWidget(new AdministratorAddUserWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Create New User"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAdministratorWidget->setObjectName(QStringLiteral("mAdministratorWidget"));
    mainLayout->addWidget(mAdministratorWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorAddUserDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorAddUserDialog::accept);
    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    buttonOk->setEnabled(false);
    connect(mAdministratorWidget, &AdministratorAddUserWidget::updateButtonOk, buttonOk, &QPushButton::setEnabled);
}

AdministratorAddUserDialog::~AdministratorAddUserDialog() = default;

RocketChatRestApi::CreateUpdateUserInfo AdministratorAddUserDialog::createInfo() const
{
    return mAdministratorWidget->createInfo();
}

RocketChatRestApi::UpdateUserInfo AdministratorAddUserDialog::updateInfo() const
{
    return mAdministratorWidget->updateInfo();
}

void AdministratorAddUserDialog::setUser(const User &user)
{
    setWindowTitle(i18nc("@title:window", "Edit User"));
    mAdministratorWidget->setUser(user);
}

void AdministratorAddUserDialog::setRoleInfo(const QList<RoleInfo> &info)
{
    mAdministratorWidget->setRoleInfo(info);
}

#include "moc_administratoradduserdialog.cpp"
