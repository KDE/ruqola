/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechwidgettest.h"
#include "abstracttexttospeechinterface.h"
#include "texttospeechsliderwidget.h"
#include "texttospeechwidget.h"
#include <QHBoxLayout>
#include <QSignalSpy>
#include <QSlider>
#include <QTest>
#include <QToolButton>
#include <qtestmouse.h>

Q_DECLARE_METATYPE(TextEditTextToSpeech::TextToSpeechWidget::State)
QTEST_MAIN(TextToSpeechWidgetTest)
TextToSpeechWidgetTest::TextToSpeechWidgetTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<TextEditTextToSpeech::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

void TextToSpeechWidgetTest::addInterface(TextEditTextToSpeech::TextToSpeechWidget *widget)
{
    auto interface = new TextEditTextToSpeech::AbstractTextToSpeechInterface(this);
    widget->setTextToSpeechInterface(interface);
}

void TextToSpeechWidgetTest::shouldHaveDefaultValue()
{
    TextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);

    auto hbox = textToSpeechWidget.findChild<QHBoxLayout *>(QStringLiteral("hbox"));
    QVERIFY(hbox);
    QCOMPARE(hbox->contentsMargins(), QMargins{});

    QCOMPARE(textToSpeechWidget.state(), TextEditTextToSpeech::TextToSpeechWidget::Stop);

    auto closeButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("close-button"));
    QVERIFY(closeButton);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!stopButton->icon().isNull());

    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton);
    QVERIFY(!playPauseButton->isEnabled());
    QVERIFY(!playPauseButton->icon().isNull());

    auto volume = textToSpeechWidget.findChild<TextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("volumeslider"));
    QVERIFY(volume);

    auto configureButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("configurebutton"));
    QVERIFY(configureButton);
    QVERIFY(!configureButton->icon().isNull());
}

void TextToSpeechWidgetTest::shouldChangeButtonEnableStateWhenChangeState()
{
    TextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton->isEnabled());

    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Pause);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Stop);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!playPauseButton->isEnabled());
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnPlayPause()
{
    TextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);

    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QCOMPARE(textToSpeechWidget.state(), TextEditTextToSpeech::TextToSpeechWidget::Play);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), TextEditTextToSpeech::TextToSpeechWidget::Pause);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), TextEditTextToSpeech::TextToSpeechWidget::Play);
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnStop()
{
    TextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), TextEditTextToSpeech::TextToSpeechWidget::Stop);
}

void TextToSpeechWidgetTest::shouldEmitStateChanged()
{
    TextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QSignalSpy spy(&textToSpeechWidget, &TextEditTextToSpeech::TextToSpeechWidget::stateChanged);
    textToSpeechWidget.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));

    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Pause);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Play);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Pause);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Stop);
}
