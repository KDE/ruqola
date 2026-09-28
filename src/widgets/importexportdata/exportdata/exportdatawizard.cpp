/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatawizard.h"

#include "exportdatafinishpage.h"
#include "exportdataselectaccountpage.h"
#include "importexportdata/importexportutils.h"
#include "managerdatapaths.h"
#include <KLocalizedString>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

#include <QDirIterator>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
using namespace std::chrono_literals;

namespace
{
const char myConfigExportDataWizardGroupName[] = "ExportDataWizard";
}
ExportDataWizard::ExportDataWizard(QWidget *parent)
    : QWizard(parent)
    , mExportDataSelectAccountPage(new ExportDataSelectAccountPage(this))
    , mExportDataFinishPage(new ExportDataFinishPage(this))
{
    setWindowTitle(i18nc("@title:window", "Export Accounts"));

    mExportDataSelectAccountPage->setObjectName(u"mExportDataSelectAccountPage"_s);
    mExportDataFinishPage->setObjectName(u"mExportDataFinishPage"_s);

    setPage(int(ExportDataWizard::ExportDataEnum::SelectAccountPage), mExportDataSelectAccountPage);
    setPage(int(ExportDataWizard::ExportDataEnum::FinishPage), mExportDataFinishPage);

    readConfig();
    loadAccountInfo();
    // Connect after loadAccountInfo
    connect(this, &ExportDataWizard::currentIdChanged, this, &ExportDataWizard::slotCurrentIdChanged);
}

ExportDataWizard::~ExportDataWizard() = default;

void ExportDataWizard::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigExportDataWizardGroupName), QSize(800, 600));
}

void ExportDataWizard::loadAccountInfo()
{
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList{u"ruqola.conf"_s},
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QList<ImportExportUtils::AccountImportExportInfo> lstAccountInfo;
    while (it.hasNext()) {
        const QString val = it.next();
        const ImportExportUtils::AccountImportExportInfo info{
            .path = val,
            .accountName = QFileInfo(val).dir().dirName(),
        };
        lstAccountInfo.append(info);
    }
    mExportDataSelectAccountPage->setAccountList(lstAccountInfo);
}

void ExportDataWizard::slotCurrentIdChanged(int id)
{
    if (id == int(ExportDataWizard::ExportDataEnum::FinishPage)) {
        mExportDataFinishPage->setListAccounts(mExportDataSelectAccountPage->selectedAccounts());
        // qDebug() << " selected account " << mExportDataSelectAccountPage->selectedAccounts();
        QTimer::singleShot(200ms, this, &ExportDataWizard::exportAccounts);
    }
}

void ExportDataWizard::exportAccounts()
{
    mExportDataFinishPage->exportAccounts();
}

#include "moc_exportdatawizard.cpp"
