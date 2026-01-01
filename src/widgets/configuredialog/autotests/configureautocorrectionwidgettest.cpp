/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureautocorrectionwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "configuredialog/configureautocorrectionwidget.h"
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureAutoCorrectionWidgetTest)
ConfigureAutoCorrectionWidgetTest::ConfigureAutoCorrectionWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureAutoCorrectionWidgetTest::shouldHaveDefaultValues()
{
    ConfigureAutoCorrectionWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    mainLayout->setContentsMargins({});
}

#include "moc_configureautocorrectionwidgettest.cpp"
