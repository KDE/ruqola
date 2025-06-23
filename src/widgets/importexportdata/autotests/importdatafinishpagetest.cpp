/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "importdatafinishpagetest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mInfos = w.findChild<QLabel *>(u"mInfos"_s);
    QVERIFY(mInfos);
    QVERIFY(mInfos->text().isEmpty());

    auto mMessageWidget = w.findChild<KMessageWidget *>(u"mMessageWidget"_s);
    QVERIFY(mMessageWidget);
    QVERIFY(!mMessageWidget->isCloseButtonVisible());

    auto mDetails = w.findChild<QPlainTextEdit *>(u"mDetails"_s);
    QVERIFY(mDetails);
    QVERIFY(mDetails->toPlainText().isEmpty());
    QVERIFY(mDetails->isReadOnly());
}

#include "moc_importdatafinishpagetest.cpp"
