/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfigurewidgettest.h"
#include "translator/widgets/translatorconfigurewidget.h"
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TranslatorConfigureWidgetTest)
TranslatorConfigureWidgetTest::TranslatorConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TranslatorConfigureWidgetTest::shouldHaveDefaultValues()
{
    PimCommonTextTranslator::TranslatorConfigureWidget w;

    auto mEngine = w.findChild<QComboBox *>(QStringLiteral("mEngine"));
    QVERIFY(mEngine);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
}
