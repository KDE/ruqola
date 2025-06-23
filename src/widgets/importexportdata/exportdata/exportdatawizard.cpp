/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatawizard.h"
using namespace Qt::Literals::StringLiterals;

#include "exportdatafinishpage.h"
#include "exportdataselectaccountpage.h"
#include "importexportdata/importexportutils.h"
#include "managerdatapaths.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

#include <QDirIterator>
#include <QTimer>
#include <QWindow>

using namespace std::chrono_literals;

namespace
{
static const char myConfigExportDataWizardGroupName[] = "ExportDataWizard";
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

ExportDataWizard::~ExportDataWizard()
{
    writeConfig();
}

void ExportDataWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigExportDataWizardGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExportDataWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigExportDataWizardGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ExportDataWizard::loadAccountInfo()
{
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << u"ruqola.conf"_s,
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    QList<ImportExportUtils::AccountImportExportInfo> lstAccountInfo;
    while (it.hasNext()) {
        const QString val = it.next();
        ImportExportUtils::AccountImportExportInfo info;
        info.path = val;
        info.accountName = QFileInfo(val).dir().dirName();
        lstAccountInfo.append(std::move(info));
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
