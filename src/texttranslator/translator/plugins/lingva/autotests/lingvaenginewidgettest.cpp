/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaenginewidgettest.h"
#include "../lingvaenginewidget.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(LingvaEngineWidgetTest)
LingvaEngineWidgetTest::LingvaEngineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LingvaEngineWidgetTest::shouldHaveDefaultValues()
{
    LingvaEngineWidget w;
    QVERIFY(w.serverUrl().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->isClearButtonEnabled());
}
