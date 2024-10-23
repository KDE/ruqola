/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdescriptiondialog.h"
#include "applicationssettingsdescriptionwidget.h"
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
ApplicationsSettingsDescriptionDialog::ApplicationsSettingsDescriptionDialog(QWidget *parent)
    : QDialog(parent)
    , mApplicationsSettingsDescriptionWidget(new ApplicationsSettingsDescriptionWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Description"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mApplicationsSettingsDescriptionWidget->setObjectName("mApplicationsSettingsDescriptionWidget"_L1);
    mainLayout->addWidget(mApplicationsSettingsDescriptionWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
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
    mApplicationsSettingsDescriptionWidget->setDescription(desc);
}

#include "moc_applicationssettingsdescriptiondialog.cpp"
