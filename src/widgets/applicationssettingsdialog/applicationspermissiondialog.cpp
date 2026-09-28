/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationspermissiondialog.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationspermissionwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

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
    readConfig();
}

ApplicationsPermissionDialog::~ApplicationsPermissionDialog() = default;

void ApplicationsPermissionDialog::setApplicationPermission(const QString &desc)
{
    mApplicationsPermissionWidget->setApplicationPermission(desc);
}

void ApplicationsPermissionDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this,
                                                             QLatin1StringView(myApplicationsSettingsApplicationsPermissionDialogGroupName),
                                                             QSize(400, 300));
}

#include "moc_applicationspermissiondialog.cpp"
