/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpagetest.h"
#include "importexportdata/importdata/importdatafinishpage.h"
#include <KMessageWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ImportDataFinishPageTest)

ImportDataFinishPageTest::ImportDataFinishPageTest(QObject *parent)
    : QObject{parent}
{
}

void ImportDataFinishPageTest::shouldHaveDefaultValues()
{
    ImportDataFinishPage w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mInfos = w.findChild<QLabel *>(QStringLiteral("mInfos"));
    QVERIFY(mInfos);
    QVERIFY(mInfos->text().isEmpty());


    auto mMessageWidget = w.findChild<KMessageWidget *>(QStringLiteral("mMessageWidget"));
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());

    auto mDetails = w.findChild<QPlainTextEdit *>(QStringLiteral("mDetails"));
    QVERIFY(mDetails);
    QVERIFY(mDetails->toPlainText().isEmpty());
    QVERIFY(mDetails->isReadOnly());
}

#include "moc_importdatafinishpagetest.cpp"
