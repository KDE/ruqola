/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurecombowidgettest.h"
#include "translator/widgets/translatorconfigurecombowidget.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(TranslatorConfigureComboWidgetTest)
TranslatorConfigureComboWidgetTest::TranslatorConfigureComboWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorConfigureComboWidgetTest::shouldHaveDefaultValues()
{
    TextTranslator::TranslatorConfigureComboWidget d;
    auto mEngineComboBox = d.findChild<QComboBox *>(QStringLiteral("mEngineComboBox"));
    QVERIFY(mEngineComboBox);
    auto mConfigureEngine = d.findChild<QToolButton *>(QStringLiteral("mConfigureEngine"));
    QVERIFY(mConfigureEngine);

    auto mainLayout = d.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    QVERIFY(!mConfigureEngine->isEnabled());
}
