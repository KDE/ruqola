/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "checknewserverurlwidgettest.h"
#include "configurenewserver/checknewserverurlwidget.h"
#include <KBusyIndicatorWidget>
#include <KMessageWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CheckNewServerUrlWidgetTest)

CheckNewServerUrlWidgetTest::CheckNewServerUrlWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CheckNewServerUrlWidgetTest::shouldHaveDefaultValues()
{
    CheckNewServerUrlWidget w;

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

#include "moc_checknewserverurlwidgettest.cpp"
