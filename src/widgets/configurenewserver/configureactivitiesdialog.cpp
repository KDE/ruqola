/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureactivitiesdialog.h"

#include "configureactivitieswidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;

namespace
{
const char myConfigConfigureActivitiesDialogGroupName[] = "ConfigureActivitiesDialog";
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

ConfigureActivitiesDialog::~ConfigureActivitiesDialog() = default;

void ConfigureActivitiesDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigConfigureActivitiesDialogGroupName), QSize(800, 600));
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
