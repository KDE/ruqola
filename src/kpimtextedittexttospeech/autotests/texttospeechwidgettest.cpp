/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

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

Q_DECLARE_METATYPE(KPIMTextEditTextToSpeech::TextToSpeechWidget::State)
QTEST_MAIN(TextToSpeechWidgetTest)
TextToSpeechWidgetTest::TextToSpeechWidgetTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

void TextToSpeechWidgetTest::addInterface(KPIMTextEditTextToSpeech::TextToSpeechWidget *widget)
{
    auto interface = new KPIMTextEditTextToSpeech::AbstractTextToSpeechInterface(this);
    widget->setTextToSpeechInterface(interface);
}

void TextToSpeechWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);

    auto hbox = textToSpeechWidget.findChild<QHBoxLayout *>(QStringLiteral("hbox"));
    QVERIFY(hbox);
    QCOMPARE(hbox->contentsMargins(), QMargins{});

    QCOMPARE(textToSpeechWidget.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);

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

    auto volume = textToSpeechWidget.findChild<KPIMTextEditTextToSpeech::TextToSpeechSliderWidget *>(QStringLiteral("volumeslider"));
    QVERIFY(volume);

    auto configureButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("configurebutton"));
    QVERIFY(configureButton);
    QVERIFY(!configureButton->icon().isNull());
}

void TextToSpeechWidgetTest::shouldChangeButtonEnableStateWhenChangeState()
{
    KPIMTextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QVERIFY(stopButton->isEnabled());

    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(playPauseButton->isEnabled());

    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);
    QVERIFY(stopButton->isEnabled());
    QVERIFY(!playPauseButton->isEnabled());
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnPlayPause()
{
    KPIMTextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);

    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
}

void TextToSpeechWidgetTest::shouldChangeStateWhenClickOnStop()
{
    KPIMTextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(textToSpeechWidget.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);
}

void TextToSpeechWidgetTest::shouldEmitStateChanged()
{
    KPIMTextEditTextToSpeech::TextToSpeechWidget textToSpeechWidget;
    addInterface(&textToSpeechWidget);
    QSignalSpy spy(&textToSpeechWidget, &KPIMTextEditTextToSpeech::TextToSpeechWidget::stateChanged);
    textToSpeechWidget.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    auto stopButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("stopbutton"));

    auto playPauseButton = textToSpeechWidget.findChild<QToolButton *>(QStringLiteral("playpausebutton"));

    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    QTest::mouseClick(playPauseButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    QTest::mouseClick(stopButton, Qt::LeftButton);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);
}
