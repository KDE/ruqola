/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myConfigAdministratorCustomUserStatusCreateDialogGroupName[] = "AdministratorCustomUserStatusCreateDialog";
}
AdministratorCustomUserStatusCreateDialog::AdministratorCustomUserStatusCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomUserStatusCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create New User Status"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomUserStatusCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomUserStatusCreateDialog::accept);
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorCustomUserStatusCreateWidget::updateOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

AdministratorCustomUserStatusCreateDialog::~AdministratorCustomUserStatusCreateDialog()
{
    writeConfig();
}

void AdministratorCustomUserStatusCreateDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigAdministratorCustomUserStatusCreateDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorCustomUserStatusCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigAdministratorCustomUserStatusCreateDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

AdministratorCustomUserStatusCreateWidget::UserStatusInfo AdministratorCustomUserStatusCreateDialog::userStatusInfo() const
{
    return mCreateWidget->userStatusInfo();
}

void AdministratorCustomUserStatusCreateDialog::setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info)
{
    mCreateWidget->setUserStatusInfo(info);
    setWindowTitle(i18nc("@title:window", "Edit User Status"));
}

#include "moc_administratorcustomuserstatuscreatedialog.cpp"
