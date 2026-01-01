/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ruqolaquicksearchmessagesettingstest.h"
// #include "core/models/textautogeneratemessagesmodel.h"
#include "model/messagesmodel.h"
#include "quicksearch/ruqolaquicksearchmessagesettings.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(RuqolaQuickSearchMessageSettingsTest)
using namespace Qt::Literals::StringLiterals;
RuqolaQuickSearchMessageSettingsTest::RuqolaQuickSearchMessageSettingsTest(QObject *parent)
    : QObject{parent}
{
}

void RuqolaQuickSearchMessageSettingsTest::shouldHaveDefaultValues()
{
    RuqolaQuickSearchMessageSettings w(nullptr);
    QCOMPARE(w.currentSearchIndex(), -1);
    QVERIFY(w.currentMessageIdentifier().isEmpty());
    QCOMPARE(w.foundSearchCount(), -1);
}

void RuqolaQuickSearchMessageSettingsTest::shouldClear()
{
    RuqolaQuickSearchMessageSettings w(nullptr);
    const QByteArray ba = "foo"_ba;
    w.setCurrentMessageIdentifier(ba);
    QCOMPARE(w.currentMessageIdentifier(), ba);

    const int currentSearchIndex = 45;
    w.setCurrentSearchIndex(currentSearchIndex);
    QCOMPARE(w.currentSearchIndex(), currentSearchIndex);

    w.clear();
    QCOMPARE(w.currentSearchIndex(), -1);
    QVERIFY(w.currentMessageIdentifier().isEmpty());
}

void RuqolaQuickSearchMessageSettingsTest::shouldTestNextSearchStringIndex()
{
    // TODO
    return;

    MessagesModel model;
    QList<Message> messages;
    {
        Message msg;
        msg.setMessageId("foo1"_ba);
        msg.setText(u"kde and kde"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    {
        Message msg;
        msg.setMessageId("foo2"_ba);
        msg.setText(u"bla bla"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    {
        Message msg;
        msg.setMessageId("foo3"_ba);
        msg.setText(u"kde"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    model.addMessages(messages);
    model.setSearchText(u"kde"_s);
    // TODO get found nb element

    RuqolaQuickSearchMessageSettings messageSettings(&model);

#if 0
    QCOMPARE(nbSearchString, 3);
    messageSettings.setFoundSearchCount(nbSearchString);
#endif
    QSignalSpy spy(&messageSettings, &RuqolaQuickSearchMessageSettings::refreshMessage);

    QSignalSpy spyUpdateButtons(&messageSettings, &RuqolaQuickSearchMessageSettings::updateNextPreviousButtons);

    messageSettings.next();
    QCOMPARE(spy.count(), 1);
    // Last one
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    QCOMPARE(spyUpdateButtons.count(), 1);
    QCOMPARE(spyUpdateButtons.at(0).at(0), false);
    QCOMPARE(spyUpdateButtons.at(0).at(1), true);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    QCOMPARE(spyUpdateButtons.count(), 2);
    QCOMPARE(spyUpdateButtons.at(1).at(0), false);
    QCOMPARE(spyUpdateButtons.at(1).at(1), true);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);
}

void RuqolaQuickSearchMessageSettingsTest::shouldTestPreviousSearchStringIndex()
{
    // TODO
    return;
    MessagesModel model;
    QList<Message> messages;
    {
        Message msg;
        msg.setMessageId("foo1"_ba);
        msg.setText(u"kde and kde"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    {
        Message msg;
        msg.setMessageId("foo2"_ba);
        msg.setText(u"bla bla"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    {
        Message msg;
        msg.setMessageId("foo3"_ba);
        msg.setText(u"kde"_s);
#if 0
        msg.generateHtml();
#endif
        messages.append(msg);
    }
    model.addMessages(messages);
    model.setSearchText(u"kde"_s);
    // TODO get found nb element
    RuqolaQuickSearchMessageSettings messageSettings(&model);
#if 0
    messageSettings.setFoundSearchCount(nbSearchString);
#endif
    QSignalSpy spy(&messageSettings, &RuqolaQuickSearchMessageSettings::refreshMessage);
    QSignalSpy spyUpdateButtons(&messageSettings, &RuqolaQuickSearchMessageSettings::updateNextPreviousButtons);
#if 0
    QCOMPARE(nbSearchString, 3);
#endif
    messageSettings.previous();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0), "foo3"_ba);
    QCOMPARE(spy.at(0).at(1), ""_ba);
    QCOMPARE(spy.at(0).at(2), 0);

    QCOMPARE(spyUpdateButtons.count(), 0);
    // Last one
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.previous();

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0), "foo1"_ba);
    QCOMPARE(spy.at(1).at(1), "foo3"_ba);
    QCOMPARE(spy.at(1).at(2), 1);

    QCOMPARE(spyUpdateButtons.count(), 1);
    QCOMPARE(spyUpdateButtons.at(0).at(0), true);
    QCOMPARE(spyUpdateButtons.at(0).at(1), true);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 1);

    messageSettings.previous();

    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0), "foo1"_ba);
    QCOMPARE(spy.at(2).at(1), "foo1"_ba);
    QCOMPARE(spy.at(2).at(2), 0);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    QCOMPARE(spyUpdateButtons.count(), 2);
    QCOMPARE(spyUpdateButtons.at(1).at(0), true);
    QCOMPARE(spyUpdateButtons.at(1).at(1), true);

    // no other search element
    messageSettings.previous();

    // No new signal
    QCOMPARE(spy.count(), 3);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    QCOMPARE(spyUpdateButtons.count(), 3);
    QCOMPARE(spyUpdateButtons.at(2).at(0), true);
    QCOMPARE(spyUpdateButtons.at(2).at(1), false);

    messageSettings.previous();
    // No new signal
    QCOMPARE(spy.count(), 3);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.previous();
    // No new signal
    QCOMPARE(spy.count(), 3);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.previous();
    // No new signal
    QCOMPARE(spy.count(), 3);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);
}

#include "moc_ruqolaquicksearchmessagesettingstest.cpp"
