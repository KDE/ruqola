/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewservercheckurlwidgettest.h"
#include "createnewserver/createnewservercheckurlwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateNewServerCheckUrlWidgetTest)

CreateNewServerCheckUrlWidgetTest::CreateNewServerCheckUrlWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateNewServerCheckUrlWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerCheckUrlWidget w;

    auto topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

    auto serverUrlLayout = w.findChild<QHBoxLayout *>(QStringLiteral("serverUrlLayout"));
    QVERIFY(serverUrlLayout);
    QCOMPARE(serverUrlLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(!label->text().isEmpty());
}

#include "moc_createnewservercheckurlwidgettest.cpp"
