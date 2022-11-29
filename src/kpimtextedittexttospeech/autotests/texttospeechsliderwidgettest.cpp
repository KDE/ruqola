/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechsliderwidgettest.h"
#include "texttospeechsliderwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QTest>
QTEST_MAIN(TextToSpeechSliderWidgetTest)
TextToSpeechSliderWidgetTest::TextToSpeechSliderWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TextToSpeechSliderWidgetTest::shouldHaveDefaultValues()
{
    KPIMTextEditTextToSpeech::TextToSpeechSliderWidget w({});
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);

    auto mSlider = w.findChild<QSlider *>(QStringLiteral("mSlider"));
    QVERIFY(mSlider);
    QCOMPARE(mSlider->orientation(), Qt::Horizontal);
}
