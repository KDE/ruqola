/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

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
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomSoundsCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorCustomSoundsCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomSoundsCreateDialogGroupName);
    group.writeEntry("Size", size());
}
