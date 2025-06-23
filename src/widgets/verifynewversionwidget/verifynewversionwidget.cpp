/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "verifynewversionwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ruqola.h"
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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mCheckVersionResultLabel->setObjectName(u"mCheckVersionResultLabel"_s);
    mCheckVersionResultLabel->setWordWrap(true);
    mCheckVersionResultLabel->setOpenExternalLinks(true);
    mCheckVersionResultLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mCheckVersionResultLabel);
}

VerifyNewVersionWidget::~VerifyNewVersionWidget() = default;

void VerifyNewVersionWidget::checkNewVersion()
{
#if RUQOLA_STABLE_VERSION
    auto job = new NeedUpdateCheckExistingNewVersionJob(this);
    job->setUrl(NeedUpdateVersionUtils::nextVersionUrl());
    job->setCompileDate(NeedUpdateVersionUtils::compileDate());
    connect(job, &NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, &VerifyNewVersionWidget::slotFoundNextVersion);
    job->start();
#else
    checkCurrentVersion();
#endif
}

void VerifyNewVersionWidget::checkCurrentVersion()
{
    auto job = new NeedUpdateCheckExistingNewVersionJob(this);
    const QUrl url = NeedUpdateVersionUtils::newVersionUrl();
    job->setUrl(url);
    job->setCompileDate(NeedUpdateVersionUtils::compileDate());
    connect(job, &NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, [this, url](bool found) {
        slotFoundNewVersion(url, found);
    });
    job->start();
}

void VerifyNewVersionWidget::slotFoundNextVersion(bool found)
{
    if (found) {
        slotFoundNewVersion(NeedUpdateVersionUtils::nextVersionUrl(), found);
    } else {
        checkCurrentVersion();
    }
}

void VerifyNewVersionWidget::slotFoundNewVersion(const QUrl &url, bool found)
{
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
