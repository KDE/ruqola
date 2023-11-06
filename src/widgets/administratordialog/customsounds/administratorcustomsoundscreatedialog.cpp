/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigAdministratorCustomSoundsCreateDialogGroupName[] = "AdministratorCustomSoundsCreateDialog";
}

AdministratorCustomSoundsCreateDialog::AdministratorCustomSoundsCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomSoundsCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create New Sound"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomSoundsCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomSoundsCreateDialog::accept);
    readConfig();
}

AdministratorCustomSoundsCreateDialog::~AdministratorCustomSoundsCreateDialog()
{
    writeConfig();
}

void AdministratorCustomSoundsCreateDialog::setCustomSoundInfo(const AdministratorCustomSoundsCreateWidget::CustomSoundInfo &info)
{
    mCreateWidget->setCustomSoundInfo(info);
}

AdministratorCustomSoundsCreateWidget::CustomSoundInfo AdministratorCustomSoundsCreateDialog::customSoundInfo() const
{
    return mCreateWidget->customSoundInfo();
}

void AdministratorCustomSoundsCreateDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigAdministratorCustomSoundsCreateDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorCustomSoundsCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigAdministratorCustomSoundsCreateDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_administratorcustomsoundscreatedialog.cpp"
