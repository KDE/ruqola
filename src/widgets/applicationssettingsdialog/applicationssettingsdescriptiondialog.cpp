/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiondialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myApplicationsSettingsDescriptionDialogGroupName[] = "ApplicationsSettingsDescriptionDialog";
}
ApplicationsSettingsDescriptionDialog::ApplicationsSettingsDescriptionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Applications"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsDescriptionDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ApplicationsSettingsDescriptionDialog::accept);
    readConfig();
}

ApplicationsSettingsDescriptionDialog::~ApplicationsSettingsDescriptionDialog()
{
    writeConfig();
}

void ApplicationsSettingsDescriptionDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsDescriptionDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsSettingsDescriptionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsDescriptionDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_applicationssettingsdescriptiondialog.cpp"
