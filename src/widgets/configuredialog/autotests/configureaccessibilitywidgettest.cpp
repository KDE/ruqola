/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidgettest.h"
#include "configuredialog/configureaccessibilitywidget.h"
#include <QTest>
#include <QVBoxLayout>
#include <TextEditTextToSpeech/TextToSpeechConfigWidget>

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

    auto mTextToSpeechWidget = w.findChild<TextEditTextToSpeech::TextToSpeechConfigWidget *>(QStringLiteral("mTextToSpeechWidget"));
    QVERIFY(mTextToSpeechWidget);
}
