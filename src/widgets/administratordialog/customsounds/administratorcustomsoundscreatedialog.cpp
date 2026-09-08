/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigAdministratorCustomSoundsCreateDialogGroupName), QSize(800, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myConfigAdministratorCustomSoundsCreateDialogGroupName), 800, 300);
#endif
}

void AdministratorCustomSoundsCreateDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigAdministratorCustomSoundsCreateDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_administratorcustomsoundscreatedialog.cpp"
