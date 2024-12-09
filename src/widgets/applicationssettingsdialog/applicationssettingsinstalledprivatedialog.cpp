/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsinstalledprivatedialog.h"
#include "applicationssettingswidgetbase.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myApplicationsSettingsInstalledPrivateDialogGroupName[] = "ApplicationsSettingsInstalledPrivateDialog";
}

ApplicationsSettingsInstalledPrivateDialog::ApplicationsSettingsInstalledPrivateDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mApplicationsSettingsWidget(new ApplicationsSettingsWidgetBase(account, this))
{
    setWindowTitle(i18nc("@title:window", "Applications"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mApplicationsSettingsWidget->setObjectName(QStringLiteral("mApplicationsSettingsWidget"));
    mainLayout->addWidget(mApplicationsSettingsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsInstalledPrivateDialog::reject);

    readConfig();
}

ApplicationsSettingsInstalledPrivateDialog::~ApplicationsSettingsInstalledPrivateDialog()
{
    writeConfig();
}

void ApplicationsSettingsInstalledPrivateDialog::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsWidget->setFeature(feature);
}

void ApplicationsSettingsInstalledPrivateDialog::initialize()
{
    mApplicationsSettingsWidget->initialize();
}

void ApplicationsSettingsInstalledPrivateDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsInstalledPrivateDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsSettingsInstalledPrivateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsInstalledPrivateDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_applicationssettingsinstalledprivatedialog.cpp"
