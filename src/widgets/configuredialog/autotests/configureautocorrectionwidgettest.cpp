/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidgettest.h"
#include "configuredialog/configureautocorrectionwidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureAutoCorrectionWidgetTest)
ConfigureAutoCorrectionWidgetTest::ConfigureAutoCorrectionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureAutoCorrectionWidgetTest::shouldHaveDefaultValues()
{
    ConfigureAutoCorrectionWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    mainLayout->setContentsMargins({});
}
