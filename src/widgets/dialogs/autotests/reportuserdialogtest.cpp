/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/reportuserdialog.h"
#include "dialogs/reportuserwidget.h"
#include <KTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ReportUserDialogTest)

ReportUserDialogTest::ReportUserDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ReportUserDialogTest::shouldHaveDefaultValues()
{
    ReportUserDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mReportUserWidget = w.findChild<ReportUserWidget *>(u"mReportUserWidget"_s);
    QVERIFY(mReportUserWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(w.message().isEmpty());

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!okButton->isEnabled());
}

void ReportUserDialogTest::shouldUpdateOkButton()
{
    ReportUserDialog w;
    auto mReportUserWidget = w.findChild<ReportUserWidget *>(u"mReportUserWidget"_s);
    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);

    auto mMessageLineEdit = mReportUserWidget->findChild<KTextEdit *>(u"mMessageLineEdit"_s);
    QVERIFY(mMessageLineEdit);
    mMessageLineEdit->setText(u"foo"_s);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(okButton->isEnabled());
}

#include "moc_reportuserdialogtest.cpp"
