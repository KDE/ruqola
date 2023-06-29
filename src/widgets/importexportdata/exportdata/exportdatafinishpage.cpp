/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpage.h"
#include "exportaccountjob.h"
#include <KLocalizedString>
#include <KMessageWidget>
#include <QDateTime>
#include <QDir>
#include <QLabel>
#include <QVBoxLayout>

ExportDataFinishPage::ExportDataFinishPage(QWidget *parent)
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
    mMessageWidget->setText(i18n("Accounts Imported."));
    mainLayout->addWidget(mMessageWidget);
}

ExportDataFinishPage::~ExportDataFinishPage() = default;

QString ExportDataFinishPage::generateExportZipFileName() const
{
    return QDir::homePath() + QLatin1Char('/') + QStringLiteral("ruqola-accountdata-%1.zip").arg(QDateTime::currentDateTime().toString());
}

void ExportDataFinishPage::setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
{
    mListAccounts = newListAccounts;
}

void ExportDataFinishPage::exportAccounts()
{
    auto job = new ExportAccountJob(generateExportZipFileName(), this);
    connect(job, &ExportAccountJob::exportDone, this, &ExportDataFinishPage::slotExportDone);
    connect(job, &ExportAccountJob::exportFailed, this, &ExportDataFinishPage::slotExportFailed);
    job->setListAccounts(mListAccounts);
    job->start();
}

void ExportDataFinishPage::slotExportDone()
{
    mMessageWidget->animatedShow();
}

void ExportDataFinishPage::slotExportFailed(const QString &msg)
{
    QString currentText = mInfos->text();
    if (!currentText.isEmpty()) {
        currentText += QLatin1Char('\n');
    }
    mInfos->setText(currentText + msg);
    // TODO
    qDebug() << " ExportDataFinishPage::slotExportFailed()" << msg;
}

#include "moc_exportdatafinishpage.cpp"
