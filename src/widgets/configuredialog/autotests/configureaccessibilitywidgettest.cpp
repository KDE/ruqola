/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccessibilitywidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTextToSpeechWidget = w.findChild<TextEditTextToSpeech::TextToSpeechConfigWidget *>(u"mTextToSpeechWidget"_s);
    QVERIFY(mTextToSpeechWidget);
}

#include "moc_configureaccessibilitywidgettest.cpp"
