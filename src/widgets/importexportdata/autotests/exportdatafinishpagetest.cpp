/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdatafinishpagetest.h"
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
    auto mInfos = w.findChild<QLabel *>(QStringLiteral("mInfos"));
    QVERIFY(mInfos);
    QVERIFY(mInfos->text().isEmpty());

    auto mMessageWidget = w.findChild<KMessageWidget *>(QStringLiteral("mMessageWidget"));
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());
    QCOMPARE(mMessageWidget->messageType(), KMessageWidget::Information);
    QVERIFY(!mMessageWidget->text().isEmpty());
    QVERIFY(mMessageWidget->wordWrap());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mDetails = w.findChild<QPlainTextEdit *>(QStringLiteral("mDetails"));
    QVERIFY(mDetails);
    QVERIFY(mDetails->toPlainText().isEmpty());
    QVERIFY(mDetails->isReadOnly());
}

#include "moc_exportdatafinishpagetest.cpp"
