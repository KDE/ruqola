/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "verifynewversionwidget.h"
#include "needupdateversion/needupdatecheckexistingnewversionjob.h"
#include "needupdateversion/needupdateversionutils.h"
#include <KLocalizedString>
#include <QVBoxLayout>

VerifyNewVersionWidget::VerifyNewVersionWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
}

VerifyNewVersionWidget::~VerifyNewVersionWidget() = default;

void VerifyNewVersionWidget::checkNewVersion()
{
    auto job = new NeedUpdateCheckExistingNewVersionJob(this);
    job->setUrl(NeedUpdateVersionUtils::newVersionUrl());
    job->setCompileDate(NeedUpdateVersionUtils::compileDate());
    // connect(job, &NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, )
    job->start();
}

#include "moc_verifynewversionwidget.cpp"
