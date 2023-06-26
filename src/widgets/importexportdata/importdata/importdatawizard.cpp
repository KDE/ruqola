/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatawizard.h"
#include "importexportdata/importdata/importdatafinishpage.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

#include <QWindow>
namespace
{
static const char myConfigImportDataWizardGroupName[] = "ImportDataWizard";
}
ImportDataWizard::ImportDataWizard(QWidget *parent)
    : QWizard(parent)
    , mImportDataSelectAccountPage(new ImportDataSelectAccountPage(this))
    , mImportDataFinishPage(new ImportDataFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Import Accounts"));
    mImportDataSelectAccountPage->setObjectName(QStringLiteral("mImportDataSelectAccountPage"));
    mImportDataFinishPage->setObjectName(QStringLiteral("mImportDataSelectAccountPage"));

    setPage(SelectAccountPage, mImportDataSelectAccountPage);
    setPage(FinishPage, mImportDataFinishPage);

    readConfig();
}

ImportDataWizard::~ImportDataWizard()
{
    writeConfig();
}

void ImportDataWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigImportDataWizardGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ImportDataWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigImportDataWizardGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_importdatawizard.cpp"
