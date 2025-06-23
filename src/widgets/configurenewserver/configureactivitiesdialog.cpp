/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitiesdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "configureactivitieswidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigConfigureActivitiesDialogGroupName[] = "ConfigureActivitiesDialog";
}
ConfigureActivitiesDialog::ConfigureActivitiesDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureActivitiesWidget(new ConfigureActivitiesWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Activities"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mConfigureActivitiesWidget->setObjectName(u"mConfigureActivitiesWidget"_s);
    mainLayout->addWidget(mConfigureActivitiesWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigureActivitiesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigureActivitiesDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ConfigureActivitiesDialog::~ConfigureActivitiesDialog()
{
    writeConfig();
}

void ConfigureActivitiesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigConfigureActivitiesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ConfigureActivitiesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigConfigureActivitiesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

AccountManager::ActivitySettings ConfigureActivitiesDialog::activitiesSettings() const
{
    return mConfigureActivitiesWidget->activitiesSettings();
}

void ConfigureActivitiesDialog::setActivitiesSettings(const AccountManager::ActivitySettings &lst)
{
    mConfigureActivitiesWidget->setActivitiesSettings(lst);
}

#include "moc_configureactivitiesdialog.cpp"
