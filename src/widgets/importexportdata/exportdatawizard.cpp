/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatawizard.h"
#include "exportdatafinishpage.h"
#include "exportdataselectaccountpage.h"
#include "managerdatapaths.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>

#include <QDirIterator>
#include <QWindow>

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

    mExportDataSelectAccountPage->setObjectName(QStringLiteral("mExportDataSelectAccountPage"));
    mExportDataFinishPage->setObjectName(QStringLiteral("mExportDataFinishPage"));

    setPage(SelectAccountPage, mExportDataSelectAccountPage);
    setPage(FinishPage, mExportDataFinishPage);

    readConfig();
    loadAccountInfo();
}

ExportDataWizard::~ExportDataWizard()
{
    writeConfig();
}

void ExportDataWizard::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigExportDataWizardGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExportDataWizard::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigExportDataWizardGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ExportDataWizard::loadAccountInfo()
{
    QStringList lst;
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()),
                    QStringList() << QStringLiteral("ruqola.conf"),
                    QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    qDebug() << "11111 " << ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    while (it.hasNext()) {
        const QString val = it.next();
        lst << val;
    }
    mExportDataSelectAccountPage->setAccountList(lst);
}

#include "moc_exportdatawizard.cpp"
