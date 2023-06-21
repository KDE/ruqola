/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurefontwidgettest.h"
#include "configuredialog/configurefontwidget.h"
#include <KFontChooser>
#include <QCheckBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureFontWidgetTest)

ConfigureFontWidgetTest::ConfigureFontWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureFontWidgetTest::shouldHaveDefaultValues()
{
    ConfigureFontWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mCustomFontCheck = w.findChild<QCheckBox *>(QStringLiteral("mCustomFontCheck"));
    QVERIFY(mCustomFontCheck);
    QVERIFY(!mCustomFontCheck->isChecked());

    auto mFontChooser = w.findChild<KFontChooser *>(QStringLiteral("mFontChooser"));
    QVERIFY(mFontChooser);
    QVERIFY(!mFontChooser->isEnabled());
}

#include "moc_configurefontwidgettest.cpp"
