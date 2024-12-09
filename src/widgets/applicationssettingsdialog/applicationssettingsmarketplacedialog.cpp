/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsmarketplacedialog.h"
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
const char myApplicationsSettingsMarketPlaceDialogGroupName[] = "ApplicationsSettingsMarketPlaceDialog";
}

ApplicationsSettingsMarketPlaceDialog::ApplicationsSettingsMarketPlaceDialog(RocketChatAccount *account, QWidget *parent)
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
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsMarketPlaceDialog::reject);

    readConfig();
}

ApplicationsSettingsMarketPlaceDialog::~ApplicationsSettingsMarketPlaceDialog()
{
    writeConfig();
}

void ApplicationsSettingsMarketPlaceDialog::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsWidget->setFeature(feature);
}

void ApplicationsSettingsMarketPlaceDialog::initialize()
{
    mApplicationsSettingsWidget->initialize();
}

void ApplicationsSettingsMarketPlaceDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsMarketPlaceDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsSettingsMarketPlaceDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsMarketPlaceDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_applicationssettingsmarketplacedialog.cpp"
