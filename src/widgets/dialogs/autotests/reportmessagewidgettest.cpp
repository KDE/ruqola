/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/reportuserwidget.h"
#include <KTextEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ReportMessageWidgetTest)
ReportMessageWidgetTest::ReportMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ReportMessageWidgetTest::shouldHaveDefaultValues()
{
    ReportUserWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto lab = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    auto mUserName = w.findChild<QLabel *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->wordWrap());
    QVERIFY(mUserName->text().isEmpty());

    auto mMessageLineEdit = w.findChild<KTextEdit *>(u"mMessageLineEdit"_s);
    QVERIFY(mMessageLineEdit);
    QVERIFY(!mMessageLineEdit->placeholderText().isEmpty());
    QVERIFY(!mMessageLineEdit->acceptRichText());

    QVERIFY(w.message().isEmpty());
}

#include "moc_reportmessagewidgettest.cpp"
