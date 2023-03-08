/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissiondialog.h"
#include "explorepermissionwidget.h"
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
ExplorePermissionDialog::ExplorePermissionDialog(QWidget *parent)
    : QDialog(parent)
    , mExporePermissionWidget(new ExplorePermissionWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Permissions"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mExporePermissionWidget->setObjectName(QStringLiteral("mExporePermissionWidget"));
    mainLayout->addWidget(mExporePermissionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExplorePermissionDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ExplorePermissionDialog::accept);

    readConfig();
}

ExplorePermissionDialog::~ExplorePermissionDialog()
{
    writeConfig();
}

void ExplorePermissionDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myExplorePermissionDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExplorePermissionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myExplorePermissionDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ExplorePermissionDialog::setPermissions(const QVector<Permission> &permissions)
{
    mExporePermissionWidget->setPermissions(permissions);
}
