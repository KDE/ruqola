/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsaskapplicationwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myApplicationsSettingsAskApplicationDialogGroupName[] = "ApplicationsSettingsAskApplicationDialog";
}
ApplicationsSettingsAskApplicationDialog::ApplicationsSettingsAskApplicationDialog(QWidget *parent)
    : QDialog(parent)
    , mApplicationsSettingsAskApplicationWidget(new ApplicationsSettingsAskApplicationWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Applications"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mApplicationsSettingsAskApplicationWidget->setObjectName(QStringLiteral("mApplicationsSettingsAskApplicationWidget"));
    mainLayout->addWidget(mApplicationsSettingsAskApplicationWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsAskApplicationDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ApplicationsSettingsAskApplicationDialog::accept);

    readConfig();
}

ApplicationsSettingsAskApplicationDialog::~ApplicationsSettingsAskApplicationDialog()
{
    writeConfig();
}

void ApplicationsSettingsAskApplicationDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsAskApplicationDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ApplicationsSettingsAskApplicationDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsAskApplicationDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

QString ApplicationsSettingsAskApplicationDialog::message() const
{
    return mApplicationsSettingsAskApplicationWidget->message();
}

#include "moc_applicationssettingsaskapplicationdialog.cpp"
