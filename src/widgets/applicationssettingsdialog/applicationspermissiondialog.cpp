/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationspermissiondialog.h"
#include "applicationspermissionwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ApplicationsPermissionDialog::ApplicationsPermissionDialog(QWidget *parent)
    : QDialog(parent)
    , mApplicationsPermissionWidget(new ApplicationsPermissionWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Permissions"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mApplicationsPermissionWidget->setObjectName(QStringLiteral("mApplicationsPermissionWidget"));
    mainLayout->addWidget(mApplicationsPermissionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);

    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsPermissionDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ApplicationsPermissionDialog::accept);
}

ApplicationsPermissionDialog::~ApplicationsPermissionDialog() = default;

void ApplicationsPermissionDialog::setApplicationPermission(const QString &desc)
{
    mApplicationsPermissionWidget->setApplicationPermission(desc);
}

#include "moc_applicationspermissiondialog.cpp"
