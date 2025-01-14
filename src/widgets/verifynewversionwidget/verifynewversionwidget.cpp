/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "verifynewversionwidget.h"
#include "needupdateversion/needupdatecheckexistingnewversionjob.h"
#include "needupdateversion/needupdateversionutils.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

VerifyNewVersionWidget::VerifyNewVersionWidget(QWidget *parent)
    : QWidget{parent}
    , mCheckVersionResultLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mCheckVersionResultLabel->setObjectName(QStringLiteral("mCheckVersionResultLabel"));
    mCheckVersionResultLabel->setWordWrap(true);
    mainLayout->addWidget(mCheckVersionResultLabel);
}

VerifyNewVersionWidget::~VerifyNewVersionWidget() = default;

void VerifyNewVersionWidget::checkNewVersion()
{
    auto job = new NeedUpdateCheckExistingNewVersionJob(this);
    job->setUrl(NeedUpdateVersionUtils::newVersionUrl());
    job->setCompileDate(NeedUpdateVersionUtils::compileDate());
    connect(job, &NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, &VerifyNewVersionWidget::slotFoundNewVersion);
    job->start();
}

void VerifyNewVersionWidget::slotFoundNewVersion(bool found)
{
    if (found) {
        mCheckVersionResultLabel->setText(i18n("A new version found. Do you want to download it?"));
    } else {
        mCheckVersionResultLabel->setText(i18n("No new version found."));
    }
}

#include "moc_verifynewversionwidget.cpp"
