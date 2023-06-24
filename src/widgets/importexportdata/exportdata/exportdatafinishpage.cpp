/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpage.h"
#include "exportaccountjob.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ExportDataFinishPage::ExportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    // TODO add Label
}

ExportDataFinishPage::~ExportDataFinishPage() = default;

void ExportDataFinishPage::setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
{
    mListAccounts = newListAccounts;
}

void ExportDataFinishPage::exportAccounts()
{
    auto job = new ExportAccountJob(QString(), this);
    connect(job, &ExportAccountJob::exportDone, this, &ExportDataFinishPage::slotExportDone);
    connect(job, &ExportAccountJob::exportFailed, this, &ExportDataFinishPage::slotExportFailed);
    job->setListAccounts(mListAccounts);
    job->start();
}

void ExportDataFinishPage::slotExportDone()
{
    // TODO
}

void ExportDataFinishPage::slotExportFailed(const QString &msg)
{
    // TODO
}

#include "moc_exportdatafinishpage.cpp"
