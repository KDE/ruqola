/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoradduserdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mAdministratorWidget->setObjectName(u"mAdministratorWidget"_s);
    mainLayout->addWidget(mAdministratorWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
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

void AdministratorAddUserDialog::setRolesInfo(const QList<RoleInfo> &info)
{
    mAdministratorWidget->setRolesInfo(info);
}

#include "moc_administratoradduserdialog.cpp"
