/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsdialog.h"

#include "applicationssettingswidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;

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

ApplicationsSettingsDialog::~ApplicationsSettingsDialog() = default;

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
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myApplicationsSettingsDialogGroupName), QSize(400, 300));
}

#include "moc_applicationssettingsdialog.cpp"
