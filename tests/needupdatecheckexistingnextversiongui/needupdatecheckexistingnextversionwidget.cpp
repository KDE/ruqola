/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "config-ruqola.h"
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <TextAddonsWidgets/NeedUpdateCheckExistingNewVersionJob>
#include <TextAddonsWidgets/NeedUpdateVersionUtils>

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
            auto job = new TextAddonsWidgets::NeedUpdateCheckExistingNewVersionJob(this);
            job->setUrl(QUrl(lineEdit->text()));
            job->setCompileDate(TextAddonsWidgets::NeedUpdateVersionUtils::compileDate());
            connect(job, &TextAddonsWidgets::NeedUpdateCheckExistingNewVersionJob::foundNewVersion, this, [plainTextEdit](bool state) {
                plainTextEdit->setPlainText(u"New version found ? %1"_s.arg(state));
            });
            job->start();
        }
    });
    qDebug() << " compile date " << TextAddonsWidgets::NeedUpdateVersionUtils::compileDate();
}

#include "moc_needupdatecheckexistingnextversionwidget.cpp"
