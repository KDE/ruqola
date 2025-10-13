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
#include <TextAddonsWidgets/NeedUpdateParseHtmlJob>
#else
#include "needupdateversion/needupdateparsehtmljob.h"
#endif

#include "needupdateparserhtmlwidget.h"
using namespace Qt::Literals::StringLiterals;
NeedUpdateParserHtmlWidget::NeedUpdateParserHtmlWidget(QWidget *parent)
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
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
            auto job = new TextAddonsWidgets::NeedUpdateParseHtmlJob(this);
            job->setUrl(QUrl(lineEdit->text()));
            connect(job, &TextAddonsWidgets::NeedUpdateParseHtmlJob::downLoadDone, this, [plainTextEdit](const QString &data) {
                plainTextEdit->setPlainText(data);
            });
            job->start();
#else
            auto job = new NeedUpdateParseHtmlJob(this);
            job->setUrl(QUrl(lineEdit->text()));
            connect(job, &NeedUpdateParseHtmlJob::downLoadDone, this, [plainTextEdit](const QString &data) {
                plainTextEdit->setPlainText(data);
            });
            job->start();
#endif
        }
    });
}

#include "moc_needupdateparserhtmlwidget.cpp"
