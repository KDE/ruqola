/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatawizard.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/importdata/importdatafinishpage.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

#include <QUrl>
#include <QWindow>
namespace
{
const char myConfigImportDataWizardGroupName[] = "ImportDataWizard";
}
ImportDataWizard::ImportDataWizard(QWidget *parent)
    : QWizard(parent)
    , mImportDataSelectAccountPage(new ImportDataSelectAccountPage(this))
    , mImportDataFinishPage(new ImportDataFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Import Accounts"));
    mImportDataSelectAccountPage->setObjectName(u"mImportDataSelectAccountPage"_s);
    mImportDataFinishPage->setObjectName(u"mImportDataSelectAccountPage"_s);

    setPage(SelectAccountPage, mImportDataSelectAccountPage);
    setPage(FinishPage, mImportDataFinishPage);

    connect(this, &ImportDataWizard::currentIdChanged, this, &ImportDataWizard::slotCurrentIdChanged);

    readConfig();
}

ImportDataWizard::~ImportDataWizard()
{
    writeConfig();
}

void ImportDataWizard::slotCurrentIdChanged(int id)
{
    if (id == FinishPage) {
        mImportDataFinishPage->setZipFileUrl(mImportDataSelectAccountPage->zipFileUrl());
    }
}

void ImportDataWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigImportDataWizardGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ImportDataWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigImportDataWizardGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_importdatawizard.cpp"
