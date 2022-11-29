/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfigwidgettest.h"
#include "abstracttexttospeechconfiginterface.h"
#include "texttospeechconfigwidget.h"
#include "texttospeechsliderwidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QSlider>
#include <QTest>

QTEST_MAIN(TextToSpeechConfigWidgetTest)

TextToSpeechConfigWidgetTest::TextToSpeechConfigWidgetTest(QObject *parent)
    : QObject(parent)
{
}

TextToSpeechConfigWidgetTest::~TextToSpeechConfigWidgetTest() = default;

void TextToSpeechConfigWidgetTest::addInterface(KPIMTextEditTextToSpeech::TextToSpeechConfigWidget *widget)
{
    auto interface = new KPIMTextEditTextToSpeech::AbstractTextToSpeechConfigInterface(this);
    widget->setTextToSpeechConfigInterface(interface);
}

void TextToSpeechConfigWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEditTextToSpeech::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    auto volume = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("volume"));
    QVERIFY(volume);

    auto rate = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("rate"));
    QVERIFY(rate);

    auto pitch = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("pitch"));
    QVERIFY(pitch);

    auto language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
    QVERIFY(language);
    // FIXME
    // QVERIFY(language->count()>0);

    auto availableEngine = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("engine"));
    QVERIFY(availableEngine);

    auto voice = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("voice"));
    QVERIFY(voice);

    auto mTestButton = textToSpeechConfigWidget.findChild<QPushButton *>(QStringLiteral("mTestButton"));
    QVERIFY(mTestButton);
}

void TextToSpeechConfigWidgetTest::shouldEmitConfigChangedWhenChangeConfigValue()
{
    KPIMTextEditTextToSpeech::TextToSpeechConfigWidget textToSpeechConfigWidget;
    addInterface(&textToSpeechConfigWidget);
    QSignalSpy spy(&textToSpeechConfigWidget, &KPIMTextEditTextToSpeech::TextToSpeechConfigWidget::configChanged);
    auto volume = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("volume"));
    volume->setValue(5);
    QCOMPARE(spy.count(), 1);

    auto rate = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("rate"));
    rate->setValue(5);
    QCOMPARE(spy.count(), 2);

    auto pitch = textToSpeechConfigWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("pitch"));
    pitch->setValue(5);
    QCOMPARE(spy.count(), 3);

    auto language = textToSpeechConfigWidget.findChild<QComboBox *>(QStringLiteral("language"));
    language->blockSignals(true);
    QStringList lst;
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    lst << QStringLiteral("foo");
    language->addItems(lst);
    language->blockSignals(false);
    language->setCurrentIndex(3);
    QCOMPARE(spy.count(), 4);
}
