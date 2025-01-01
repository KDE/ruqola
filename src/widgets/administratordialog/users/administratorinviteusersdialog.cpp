/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinviteusersdialog.h"
#include "administratorinviteuserswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

AdministratorInviteUsersDialog::AdministratorInviteUsersDialog(QWidget *parent)
    : QDialog(parent)
    , mInviteUsersWidget(new AdministratorInviteUsersWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Send Emails Invite"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mInviteUsersWidget->setObjectName(QStringLiteral("mInviteUsersWidget"));
    mainLayout->addWidget(mInviteUsersWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorInviteUsersDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorInviteUsersDialog::accept);
    QPushButton *buttonOk = button->button(QDialogButtonBox::Ok);
    buttonOk->setEnabled(false);
    connect(mInviteUsersWidget, &AdministratorInviteUsersWidget::updateButtonOk, buttonOk, &QPushButton::setEnabled);
}

AdministratorInviteUsersDialog::~AdministratorInviteUsersDialog() = default;

QStringList AdministratorInviteUsersDialog::emails() const
{
    return mInviteUsersWidget->emails();
}

#include "moc_administratorinviteusersdialog.cpp"
