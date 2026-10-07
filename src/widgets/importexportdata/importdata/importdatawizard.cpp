/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatawizard.h"

#include "importexportdata/importdata/importdatafinishpage.h"
#include "importexportdata/importdata/importdataselectaccountpage.h"

#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

#include <QUrl>
namespace
{
const char myConfigImportDataWizardGroupName[] = "ImportDataWizard";
}
using namespace Qt::Literals::StringLiterals;
ImportDataWizard::ImportDataWizard(QWidget *parent)
    : QWizard(parent)
    , mImportDataSelectAccountPage(new ImportDataSelectAccountPage(this))
    , mImportDataFinishPage(new ImportDataFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Import Accounts"));
    mImportDataSelectAccountPage->setObjectName(u"mImportDataSelectAccountPage"_s);
    mImportDataFinishPage->setObjectName(u"mImportDataFinishPage"_s);

    setPage(SelectAccountPage, mImportDataSelectAccountPage);
    setPage(FinishPage, mImportDataFinishPage);

    connect(this, &ImportDataWizard::currentIdChanged, this, &ImportDataWizard::slotCurrentIdChanged);

    readConfig();
}

ImportDataWizard::~ImportDataWizard() = default;

void ImportDataWizard::slotCurrentIdChanged(int id)
{
    if (id == FinishPage) {
        mImportDataFinishPage->setZipFileUrl(mImportDataSelectAccountPage->zipFileUrl());
        mImportDataSelectAccountPage->setCommitPage(true);
    }
}

void ImportDataWizard::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigImportDataWizardGroupName), QSize(800, 600));
}

#include "moc_importdatawizard.cpp"
