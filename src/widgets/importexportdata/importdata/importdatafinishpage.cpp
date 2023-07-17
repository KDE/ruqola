/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpage.h"
#include "importaccountjob.h"
#include <KLocalizedString>
#include <KMessageWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QUrl>
#include <QVBoxLayout>

ImportDataFinishPage::ImportDataFinishPage(QWidget *parent)
    : QWizardPage(parent)
    , mInfos(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
    , mDetails(new QPlainTextEdit(this))
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
    mMessageWidget->setText(i18n("Accounts imported."));
    mainLayout->addWidget(mMessageWidget);

    mDetails->setObjectName(QStringLiteral("mDetails"));
    mDetails->setReadOnly(true);
    mainLayout->addWidget(mDetails);
}

ImportDataFinishPage::~ImportDataFinishPage() = default;

void ImportDataFinishPage::setZipFileUrl(const QUrl &url)
{
    mImportDone = false;
    Q_EMIT completeChanged();
    auto job = new ImportAccountJob(url.toLocalFile(), this);
    connect(job, &ImportAccountJob::importDone, this, &ImportDataFinishPage::slotImportDone);
    connect(job, &ImportAccountJob::importFailed, this, &ImportDataFinishPage::slotImportFailed);
    connect(job, &ImportAccountJob::importInfo, this, &ImportDataFinishPage::slotImportInfo);
    connect(job, &ImportAccountJob::finished, this, [this]() {
        mImportDone = true;
        Q_EMIT completeChanged();
    });
    job->start();
}

bool ImportDataFinishPage::isComplete() const
{
    return mImportDone;
}

void ImportDataFinishPage::slotImportDone()
{
    mMessageWidget->animatedShow();
}

void ImportDataFinishPage::slotImportInfo(const QString &msg)
{
    mDetails->appendPlainText(msg);
}

void ImportDataFinishPage::slotImportFailed(const QString &msg)
{
    QString currentText = mInfos->text();
    if (!currentText.isEmpty()) {
        currentText += QLatin1Char('\n');
    }
    mInfos->setText(currentText + msg);
}

#include "moc_importdatafinishpage.cpp"
