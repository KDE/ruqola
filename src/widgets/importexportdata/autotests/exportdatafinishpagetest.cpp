/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpagetest.h"
using namespace Qt::Literals::StringLiterals;

#include "importexportdata/exportdata/exportdatafinishpage.h"
#include <KMessageWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ExportDataFinishPageTest)
ExportDataFinishPageTest::ExportDataFinishPageTest(QObject *parent)
    : QObject{parent}
{
}

void ExportDataFinishPageTest::shouldHaveDefaultValues()
{
    ExportDataFinishPage w;
    auto mInfos = w.findChild<QLabel *>(u"mInfos"_s);
    QVERIFY(mInfos);
    QVERIFY(mInfos->text().isEmpty());

    auto mMessageWidget = w.findChild<KMessageWidget *>(u"mMessageWidget"_s);
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Information);
    QVERIFY(!mMessageWidget->text().isEmpty());
    QVERIFY(mMessageWidget->wordWrap());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mDetails = w.findChild<QPlainTextEdit *>(u"mDetails"_s);
    QVERIFY(mDetails);
    QVERIFY(mDetails->toPlainText().isEmpty());
    QVERIFY(mDetails->isReadOnly());
}

#include "moc_exportdatafinishpagetest.cpp"
