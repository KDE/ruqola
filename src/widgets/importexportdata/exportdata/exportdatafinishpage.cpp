/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpage.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mInfos->setObjectName(u"mInfos"_s);
    mainLayout->addWidget(mInfos);
    mInfos->setVisible(false);

    mDetails->setObjectName(u"mDetails"_s);
    mDetails->setReadOnly(true);

    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(false);
    mMessageWidget->setMessageType(KMessageWidget::Information);
    mMessageWidget->setWordWrap(true);
    mMessageWidget->setText(i18n("Accounts Exported."));
    mMessageWidget->setTextFormat(Qt::PlainText);
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
    return QDir::homePath() + u'/' + u"ruqola-accountdata-%1.zip"_s.arg(QDateTime::currentDateTime().toString());
}

void ExportDataFinishPage::setListAccounts(const QList<ImportExportUtils::AccountImportExportInfo> &newListAccounts)
{
    // We will export => it's not done yet
    mExportDone = false;
    Q_EMIT completeChanged();
    mListAccounts = newListAccounts;
}

void ExportDataFinishPage::exportAccounts()
{
    const QString fileNamePath = generateExportZipFileName();
    auto job = new ExportAccountJob(generateExportZipFileName(), this);
    connect(job, &ExportAccountJob::exportDone, this, &ExportDataFinishPage::slotExportDone);
    connect(job, &ExportAccountJob::exportFailed, this, &ExportDataFinishPage::slotExportFailed);
    connect(job, &ExportAccountJob::exportInfo, this, &ExportDataFinishPage::slotExportInfo);
    connect(job, &ExportAccountJob::finished, this, [this, fileNamePath]() {
        slotExportInfo(i18n("Generated Zip: %1", fileNamePath) + u'\n');
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
        currentText += u'\n';
    }
    mInfos->setText(currentText + msg);
    mInfos->setVisible(true);
}

void ExportDataFinishPage::slotExportInfo(const QString &msg)
{
    mDetails->appendPlainText(msg);
}

#include "moc_exportdatafinishpage.cpp"
