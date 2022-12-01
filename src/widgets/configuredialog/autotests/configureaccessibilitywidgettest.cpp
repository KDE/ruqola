/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidgettest.h"
#include "configuredialog/configureaccessibilitywidget.h"
#include <KPIMTextEditTextToSpeech/TextToSpeechConfigWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ConfigureAccessibilityWidgetTest)
ConfigureAccessibilityWidgetTest::ConfigureAccessibilityWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ConfigureAccessibilityWidgetTest::shouldHaveDefaultValues()
{
    ConfigureAccessibilityWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextToSpeechWidget = w.findChild<KPIMTextEditTextToSpeech::TextToSpeechConfigWidget *>(QStringLiteral("mTextToSpeechWidget"));
    QVERIFY(mTextToSpeechWidget);
}
