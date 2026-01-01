/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "checknewserverurlwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto topLayout = w.findChild<QVBoxLayout *>(u"topLayout"_s);
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

    auto serverUrlLayout = w.findChild<QHBoxLayout *>(u"serverUrlLayout"_s);
    QVERIFY(serverUrlLayout);
    QCOMPARE(serverUrlLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(!label->text().isEmpty());

    auto mServerUrl = w.findChild<QLineEdit *>(u"mServerUrl"_s);
    QVERIFY(mServerUrl->text().isEmpty());

    auto mBusyIndicatorWidget = w.findChild<KBusyIndicatorWidget *>(u"mBusyIndicatorWidget"_s);
    QVERIFY(mBusyIndicatorWidget);

    auto mFailedError = w.findChild<KMessageWidget *>(u"mFailedError"_s);
    QVERIFY(mFailedError);
    QCOMPARE(mFailedError->messageType(), KMessageWidget::Error);
}

#include "moc_checknewserverurlwidgettest.cpp"
