/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpage.h"
#include "importaccountjob.h"
#include <KLocalizedString>
#include <QLabel>
#include <QUrl>
#include <QVBoxLayout>

ImportDataFinishPage::ImportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
    , mInfos(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mInfos->setObjectName(QStringLiteral("mInfos"));
    mainLayout->addWidget(mInfos);
}

ImportDataFinishPage::~ImportDataFinishPage() = default;

void ImportDataFinishPage::setZipFileUrl(const QUrl &url)
{
    auto job = new ImportAccountJob(url.toLocalFile(), this);
    connect(job, &ImportAccountJob::importDone, this, &ImportDataFinishPage::slotImportDone);
    connect(job, &ImportAccountJob::importFailed, this, &ImportDataFinishPage::slotImportFailed);
    job->start();
}

void ImportDataFinishPage::slotImportDone()
{
    // TODO
}

void ImportDataFinishPage::slotImportFailed(const QString &msg)
{
    // TODO
}

#include "moc_importdatafinishpage.cpp"
