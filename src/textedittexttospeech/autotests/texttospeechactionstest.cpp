/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechactionstest.h"
#include "../texttospeechactions.h"
#include <QAction>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(TextEditTextToSpeech::TextToSpeechWidget::State)
QTEST_MAIN(TextToSpeechActionsTest)
TextToSpeechActionsTest::TextToSpeechActionsTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<TextEditTextToSpeech::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

TextToSpeechActionsTest::~TextToSpeechActionsTest() = default;

void TextToSpeechActionsTest::shouldHaveDefaultValue()
{
    TextEditTextToSpeech::TextToSpeechActions act;
    QVERIFY(act.stopAction());
    QVERIFY(act.playPauseAction());
    QCOMPARE(act.state(), TextEditTextToSpeech::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.stopAction()->icon().isNull());

    QVERIFY(!act.playPauseAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->icon().isNull());
}

void TextToSpeechActionsTest::shouldChangeButtonEnableStateWhenChangeState()
{
    TextEditTextToSpeech::TextToSpeechActions act;
    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Pause);
    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->isEnabled());
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnPlayPause()
{
    TextEditTextToSpeech::TextToSpeechActions act;
    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(act.state(), TextEditTextToSpeech::TextToSpeechWidget::Play);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), TextEditTextToSpeech::TextToSpeechWidget::Pause);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), TextEditTextToSpeech::TextToSpeechWidget::Play);
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnStop()
{
    TextEditTextToSpeech::TextToSpeechActions act;
    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);

    act.stopAction()->trigger();
    QCOMPARE(act.state(), TextEditTextToSpeech::TextToSpeechWidget::Stop);
}

void TextToSpeechActionsTest::shouldEmitStateChanged()
{
    TextEditTextToSpeech::TextToSpeechActions act;
    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    QSignalSpy spy(&act, &TextEditTextToSpeech::TextToSpeechActions::stateChanged);
    act.setState(TextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Pause);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Play);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Pause);
    act.stopAction()->trigger();
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<TextEditTextToSpeech::TextToSpeechWidget::State>(), TextEditTextToSpeech::TextToSpeechWidget::Stop);
}
