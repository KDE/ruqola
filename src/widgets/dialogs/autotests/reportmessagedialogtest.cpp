/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagedialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/reportmessagedialog.h"
#include "dialogs/reportmessagewidget.h"
#include <KTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ReportMessageDialogTest)

ReportMessageDialogTest::ReportMessageDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ReportMessageDialogTest::shouldHaveDefaultValues()
{
    ReportMessageDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mReportMessageWidget = w.findChild<ReportMessageWidget *>(u"mReportMessageWidget"_s);
    QVERIFY(mReportMessageWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(w.message().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ReportMessageDialogTest::shouldUpdateOkButton()
{
    ReportMessageDialog w;
    auto mReportMessageWidget = w.findChild<ReportMessageWidget *>(u"mReportMessageWidget"_s);
    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);

    auto mMessageLineEdit = mReportMessageWidget->findChild<KTextEdit *>(u"mMessageLineEdit"_s);
    QVERIFY(mMessageLineEdit);
    mMessageLineEdit->setText(u"foo"_s);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());
}

#include "moc_reportmessagedialogtest.cpp"
