/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiondialog.h"
#include "applicationssettingsdescriptiontabwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
using namespace Qt::Literals::StringLiterals;
namespace
{
const char myApplicationsSettingsDescriptionDialogGroupName[] = "ApplicationsSettingsDescriptionDialog";
}
ApplicationsSettingsDescriptionDialog::ApplicationsSettingsDescriptionDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mApplicationsSettingsDescriptionTabWidget(new ApplicationsSettingsDescriptionTabWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Description"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mApplicationsSettingsDescriptionTabWidget->setObjectName("mApplicationsSettingsDescriptionWidget"_L1);
    mainLayout->addWidget(mApplicationsSettingsDescriptionTabWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsDescriptionDialog::reject);
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

void ApplicationsSettingsDescriptionDialog::setDescription(const QString &desc)
{
    mApplicationsSettingsDescriptionTabWidget->setDescription(desc);
}

void ApplicationsSettingsDescriptionDialog::setApplicationId(const QByteArray &appId)
{
    mApplicationsSettingsDescriptionTabWidget->setApplicationId(appId);
}

void ApplicationsSettingsDescriptionDialog::setShowLogAndSettingsInfo(bool state)
{
    mApplicationsSettingsDescriptionTabWidget->setShowLogAndSettingsInfo(state);
}

void ApplicationsSettingsDescriptionDialog::setShowVersionInfo(bool state)
{
    mApplicationsSettingsDescriptionTabWidget->setShowVersionInfo(state);
}

#include "moc_applicationssettingsdescriptiondialog.cpp"
