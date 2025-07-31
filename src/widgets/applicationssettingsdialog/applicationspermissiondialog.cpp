/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationspermissiondialog.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationspermissionwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myApplicationsSettingsApplicationsPermissionDialogGroupName[] = "ApplicationsPermissionDialog";
}

ApplicationsPermissionDialog::ApplicationsPermissionDialog(QWidget *parent)
    : QDialog(parent)
    , mApplicationsPermissionWidget(new ApplicationsPermissionWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Permissions"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mApplicationsPermissionWidget->setObjectName(u"mApplicationsPermissionWidget"_s);
    mainLayout->addWidget(mApplicationsPermissionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);

    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsPermissionDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ApplicationsPermissionDialog::accept);
    writeConfig();
}

ApplicationsPermissionDialog::~ApplicationsPermissionDialog()
{
    writeConfig();
}

void ApplicationsPermissionDialog::setApplicationPermission(const QString &desc)
{
    mApplicationsPermissionWidget->setApplicationPermission(desc);
}

void ApplicationsPermissionDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsApplicationsPermissionDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsPermissionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsApplicationsPermissionDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_applicationspermissiondialog.cpp"
