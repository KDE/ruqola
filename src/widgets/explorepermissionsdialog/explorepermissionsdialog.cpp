/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionsdialog.h"
#include "explorepermissionswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myExplorePermissionDialogConfigGroupName[] = "ExplorePermissionDialog";
}
ExplorePermissionsDialog::ExplorePermissionsDialog(QWidget *parent)
    : QDialog(parent)
    , mExporePermissionWidget(new ExplorePermissionsWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Permissions"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mExporePermissionWidget->setObjectName(QStringLiteral("mExporePermissionWidget"));
    mainLayout->addWidget(mExporePermissionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExplorePermissionsDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ExplorePermissionsDialog::accept);

    readConfig();
}

ExplorePermissionsDialog::~ExplorePermissionsDialog()
{
    writeConfig();
}

void ExplorePermissionsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myExplorePermissionDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExplorePermissionsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myExplorePermissionDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ExplorePermissionsDialog::setPermissions(const QVector<Permission> &permissions)
{
    mExporePermissionWidget->setPermissions(permissions);
}

void ExplorePermissionsDialog::setOWnRoles(const QStringList &roleStr)
{
    mExporePermissionWidget->setOWnRoles(roleStr);
}

#include "moc_explorepermissionsdialog.cpp"
