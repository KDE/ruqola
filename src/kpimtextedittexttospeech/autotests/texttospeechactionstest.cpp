/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechactionstest.h"
#include "../texttospeechactions.h"
#include <QAction>
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(KPIMTextEditTextToSpeech::TextToSpeechWidget::State)
QTEST_MAIN(TextToSpeechActionsTest)
TextToSpeechActionsTest::TextToSpeechActionsTest(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>();
    QIcon::setThemeName(QStringLiteral("breeze"));
}

TextToSpeechActionsTest::~TextToSpeechActionsTest() = default;

void TextToSpeechActionsTest::shouldHaveDefaultValue()
{
    KPIMTextEditTextToSpeech::TextToSpeechActions act;
    QVERIFY(act.stopAction());
    QVERIFY(act.playPauseAction());
    QCOMPARE(act.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.stopAction()->icon().isNull());

    QVERIFY(!act.playPauseAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->icon().isNull());
}

void TextToSpeechActionsTest::shouldChangeButtonEnableStateWhenChangeState()
{
    KPIMTextEditTextToSpeech::TextToSpeechActions act;
    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(act.playPauseAction()->isEnabled());

    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);

    QVERIFY(act.stopAction()->isEnabled());
    QVERIFY(!act.playPauseAction()->isEnabled());
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnPlayPause()
{
    KPIMTextEditTextToSpeech::TextToSpeechActions act;
    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(act.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);

    act.playPauseAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
}

void TextToSpeechActionsTest::shouldChangeStateWhenClickOnStop()
{
    KPIMTextEditTextToSpeech::TextToSpeechActions act;
    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);

    act.stopAction()->trigger();
    QCOMPARE(act.state(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);
}

void TextToSpeechActionsTest::shouldEmitStateChanged()
{
    KPIMTextEditTextToSpeech::TextToSpeechActions act;
    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    QSignalSpy spy(&act, &KPIMTextEditTextToSpeech::TextToSpeechActions::stateChanged);
    act.setState(KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    QCOMPARE(spy.count(), 0);

    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Play);
    act.playPauseAction()->trigger();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Pause);
    act.stopAction()->trigger();
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).value<KPIMTextEditTextToSpeech::TextToSpeechWidget::State>(), KPIMTextEditTextToSpeech::TextToSpeechWidget::Stop);
}
