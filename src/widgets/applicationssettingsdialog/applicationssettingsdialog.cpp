/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
#include <TextAddonsWidgets/LoadDialogSizeUtils>

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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myApplicationsSettingsDialogGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myApplicationsSettingsDialogGroupName), 400, 300);
#endif
}

void ApplicationsSettingsDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myApplicationsSettingsDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_applicationssettingsdialog.cpp"
