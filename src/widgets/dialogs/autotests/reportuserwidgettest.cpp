/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportuserwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/reportmessagewidget.h"
#include <KTextEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ReportUserWidgetTest)
ReportUserWidgetTest::ReportUserWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ReportUserWidgetTest::shouldHaveDefaultValues()
{
    ReportMessageWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto lab = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    auto mMessagePreview = w.findChild<QLabel *>(u"mMessagePreview"_s);
    QVERIFY(mMessagePreview);
    QVERIFY(mMessagePreview->wordWrap());
    QVERIFY(mMessagePreview->text().isEmpty());

    auto mMessageLineEdit = w.findChild<KTextEdit *>(u"mMessageLineEdit"_s);
    QVERIFY(mMessageLineEdit);
    QVERIFY(!mMessageLineEdit->placeholderText().isEmpty());
    QVERIFY(!mMessageLineEdit->acceptRichText());

    QVERIFY(w.message().isEmpty());
}

#include "moc_reportuserwidgettest.cpp"
