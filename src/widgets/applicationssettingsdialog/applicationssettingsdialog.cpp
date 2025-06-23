/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingswidget.h"
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
const char myApplicationsSettingsDialogGroupName[] = "ApplicationsSettingsDialog";
}

ApplicationsSettingsDialog::ApplicationsSettingsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mApplicationsSettingsWidget(new ApplicationsSettingsWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Applications"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mApplicationsSettingsWidget->setObjectName(u"mApplicationsSettingsWidget"_s);
    mainLayout->addWidget(mApplicationsSettingsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsDialog::reject);

    readConfig();
}

ApplicationsSettingsDialog::~ApplicationsSettingsDialog()
{
    writeConfig();
}

void ApplicationsSettingsDialog::setFeature(ApplicationsSettingsSearchWidget::Feature feature)
{
    mApplicationsSettingsWidget->setFeature(feature);
}

void ApplicationsSettingsDialog::initialize()
{
    mApplicationsSettingsWidget->initialize();
}

void ApplicationsSettingsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_applicationssettingsdialog.cpp"
