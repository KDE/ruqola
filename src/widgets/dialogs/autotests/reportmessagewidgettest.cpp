/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reportmessagewidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto lab = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(lab);
    QVERIFY(!lab->text().isEmpty());

    auto mUserName = w.findChild<QLabel *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->wordWrap());
    QVERIFY(mUserName->text().isEmpty());

    auto mMessageLineEdit = w.findChild<KTextEdit *>(QStringLiteral("mMessageLineEdit"));
    QVERIFY(mMessageLineEdit);
    QVERIFY(!mMessageLineEdit->placeholderText().isEmpty());
    QVERIFY(!mMessageLineEdit->acceptRichText());

    QVERIFY(w.message().isEmpty());
}

#include "moc_reportmessagewidgettest.cpp"
