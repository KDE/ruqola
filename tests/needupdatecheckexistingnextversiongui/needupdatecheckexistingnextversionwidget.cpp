/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "config-ruqola.h"
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include <TextAddonsWidgets/NeedUpdateCheckExistingNewVersionJob>
#include <TextAddonsWidgets/NeedUpdateVersionUtils>
#else
#include "needupdateversion/needupdatecheckexistingnewversionjob.h"
#include "needupdateversion/needupdateversionutils.h"
#endif

#include "needupdatecheckexistingnextversionwidget.h"
using namespace Qt::Literals::StringLiterals;
NeedUpdateCheckExistingNextVersionWidget::NeedUpdateCheckExistingNextVersionWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    auto lineEdit = new QLineEdit(this);
    mainLayout->addWidget(lineEdit);

    auto pushButton = new QPushButton(u"Download"_s, this);
    mainLayout->addWidget(pushButton);

    auto plainTextEdit = new QPlainTextEdit(this);
    mainLayout->addWidget(plainTextEdit);
    connect(pushButton, &QPushButton::clicked, this, [this, lineEdit, plainTextEdit]() {
        if (!lineEdit->text().isEmpty()) {
            plainTextEdit->clear();
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
            auto job = new TextAddonsWidgets::NeedUpdateCheckExistingNewVersionJob(this);
            job->setUrl(QUrl(lineEdit->text()));
            job->setCompileDate(TextAddonsWidgets::NeedUpdateVersionUtils::compileDate());
            connect(job, &TextAddonsWidgets::NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, [plainTextEdit](bool state) {
                plainTextEdit->setPlainText(u"New version found ? %1"_s.arg(state));
            });
            job->start();
#else
            auto job = new NeedUpdateCheckExistingNewVersionJob(this);
            job->setUrl(QUrl(lineEdit->text()));
            job->setCompileDate(NeedUpdateVersionUtils::compileDate());
            connect(job, &NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, [plainTextEdit](bool state) {
                plainTextEdit->setPlainText(u"New version found ? %1"_s.arg(state));
            });
            job->start();
#endif
        }
    });
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    qDebug() << " compile date " << TextAddonsWidgets::NeedUpdateVersionUtils::compileDate();
#else
    qDebug() << " compile date " << NeedUpdateVersionUtils::compileDate();
#endif
}

#include "moc_needupdatecheckexistingnextversionwidget.cpp"
