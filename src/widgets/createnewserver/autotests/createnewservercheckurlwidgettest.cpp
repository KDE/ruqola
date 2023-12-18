/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewservercheckurlwidgettest.h"
#include "createnewserver/createnewservercheckurlwidget.h"
#include <KBusyIndicatorWidget>
#include <KMessageWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
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

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl->text().isEmpty());

    auto mBusyIndicatorWidget = w.findChild<KBusyIndicatorWidget *>(QStringLiteral("mBusyIndicatorWidget"));
    QVERIFY(mBusyIndicatorWidget);

    auto mFailedError = w.findChild<KMessageWidget *>(QStringLiteral("mFailedError"));
    QVERIFY(mFailedError);
    QCOMPARE(mFailedError->messageType(), KMessageWidget::Error);
}

#include "moc_createnewservercheckurlwidgettest.cpp"
