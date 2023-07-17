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
#include <QPlainTextEdit>
#include <QVBoxLayout>

ExportDataFinishPage::ExportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
    , mInfos(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
    , mDetails(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mInfos->setObjectName(QStringLiteral("mInfos"));
    mainLayout->addWidget(mInfos);

    mDetails->setObjectName(QStringLiteral("mDetails"));
    mDetails->setReadOnly(true);

    mMessageWidget->setObjectName(QStringLiteral("mMessageWidget"));
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);
    mMessageWidget->setText(i18n("Accounts Imported."));
    mainLayout->addWidget(mMessageWidget);

    mainLayout->addWidget(mDetails);
}

ExportDataFinishPage::~ExportDataFinishPage() = default;

bool ExportDataFinishPage::isComplete() const
{
    return mExportDone;
}

QString ExportDataFinishPage::generateExportZipFileName() const
{
    return QDir::homePath() + QLatin1Char('/') + QStringLiteral("ruqola-accountdata-%1.zip").arg(QDateTime::currentDateTime().toString());
}

void ExportDataFinishPage::setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
{
    // We will export => it's not done yet
    mExportDone = false;
    Q_EMIT completeChanged();
    mListAccounts = newListAccounts;
}

void ExportDataFinishPage::exportAccounts()
{
    auto job = new ExportAccountJob(generateExportZipFileName(), this);
    connect(job, &ExportAccountJob::exportDone, this, &ExportDataFinishPage::slotExportDone);
    connect(job, &ExportAccountJob::exportFailed, this, &ExportDataFinishPage::slotExportFailed);
    connect(job, &ExportAccountJob::exportInfo, this, &ExportDataFinishPage::slotExportInfo);
    connect(job, &ExportAccountJob::finished, this, [this]() {
        mExportDone = true;
        Q_EMIT completeChanged();
    });
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
}

void ExportDataFinishPage::slotExportInfo(const QString &msg)
{
    mDetails->appendPlainText(msg);
}

#include "moc_exportdatafinishpage.cpp"
