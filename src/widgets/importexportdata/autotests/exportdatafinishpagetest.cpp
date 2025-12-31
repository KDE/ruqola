/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpagetest.h"

#include "importexportdata/exportdata/exportdatafinishpage.h"
#include <KMessageWidget>
#include <QLabel>
#include <QTest>
#include <QTextBrowser>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
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

    auto mDetails = w.findChild<QTextBrowser *>(u"mDetails"_s);
    QVERIFY(mDetails);
    QVERIFY(mDetails->toPlainText().isEmpty());
}

#include "moc_exportdatafinishpagetest.cpp"
