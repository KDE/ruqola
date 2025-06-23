/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationssettingsaskapplicationdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsaskapplicationwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
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
    setWindowTitle(i18nc("@title:window", "Ask Application"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mApplicationsSettingsAskApplicationWidget->setObjectName(u"mApplicationsSettingsAskApplicationWidget"_s);
    mainLayout->addWidget(mApplicationsSettingsAskApplicationWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(button, &QDialogButtonBox::rejected, this, &ApplicationsSettingsAskApplicationDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ApplicationsSettingsAskApplicationDialog::accept);
    connect(mApplicationsSettingsAskApplicationWidget, &ApplicationsSettingsAskApplicationWidget::updateOkButton, okButton, &QPushButton::setEnabled);

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

void ApplicationsSettingsAskApplicationDialog::setApplicationName(const QString &appName)
{
    mApplicationsSettingsAskApplicationWidget->setApplicationName(appName);
}

#include "moc_applicationssettingsaskapplicationdialog.cpp"
