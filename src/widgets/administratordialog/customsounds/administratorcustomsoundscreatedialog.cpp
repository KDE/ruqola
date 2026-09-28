/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomsoundscreatedialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myConfigAdministratorCustomSoundsCreateDialogGroupName[] = "AdministratorCustomSoundsCreateDialog";
}

using namespace Qt::Literals::StringLiterals;
AdministratorCustomSoundsCreateDialog::AdministratorCustomSoundsCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomSoundsCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create New Sound"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateWidget->setObjectName(u"mCreateWidget"_s);
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomSoundsCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomSoundsCreateDialog::accept);
    readConfig();
}

AdministratorCustomSoundsCreateDialog::~AdministratorCustomSoundsCreateDialog() = default;

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
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigAdministratorCustomSoundsCreateDialogGroupName), QSize(800, 300));
}

#include "moc_administratorcustomsoundscreatedialog.cpp"
