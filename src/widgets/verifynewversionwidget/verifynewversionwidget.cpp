/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "verifynewversionwidget.h"
#include "needupdateversion/needupdatecheckexistingnewversionjob.h"
#include "needupdateversion/needupdateversionutils.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QDesktopServices>
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
    mCheckVersionResultLabel->setOpenExternalLinks(true);
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
    const QUrl url = NeedUpdateVersionUtils::newVersionUrl();
    mCheckVersionResultLabel->setTextFormat(Qt::RichText);
    if (found) {
        mCheckVersionResultLabel->setText(i18n("A new version found. Click <a href=\"%1\">here</a> for downloading it.", url.toString()));
    } else {
        mCheckVersionResultLabel->setText(i18n("No new version found. Build can be found <a href=\"%1\">here</a>.", url.toString()));
    }
    connect(mCheckVersionResultLabel, &QLabel::linkActivated, this, [](const QString &url) {
        if (!QDesktopServices::openUrl(QUrl(url))) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to open url: " << url;
        }
    });
}

#include "moc_verifynewversionwidget.cpp"
