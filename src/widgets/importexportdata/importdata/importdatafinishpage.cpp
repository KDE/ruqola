/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpage.h"
#include "importaccountjob.h"
#include <KLocalizedString>
#include <QDebug>
#include <QLabel>
#include <QUrl>
#include <QVBoxLayout>
#include <KMessageWidget>

ImportDataFinishPage::ImportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
    , mInfos(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mInfos->setObjectName(QStringLiteral("mInfos"));
    mainLayout->addWidget(mInfos);

    mMessageWidget->setObjectName(QStringLiteral("mMessageWidget"));
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);
    mMessageWidget->setText(i18n("Accounts Exported."));
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
    mMessageWidget->animatedShow();
}

void ImportDataFinishPage::slotImportFailed(const QString &msg)
{
    QString currentText = mInfos->text();
    if (!currentText.isEmpty()) {
        currentText += QLatin1Char('\n');
    }
    mInfos->setText(currentText + msg);
    // TODO
    qDebug() << " ExportDataFinishPage::slotExportFailed()" << msg;
}

#include "moc_importdatafinishpage.cpp"
