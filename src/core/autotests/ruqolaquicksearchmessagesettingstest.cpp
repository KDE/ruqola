/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ruqolaquicksearchmessagesettingstest.h"
// #include "core/models/textautogeneratemessagesmodel.h"
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
#if 0
    TextAutoGenerateText::TextAutoGenerateMessagesModel model;
    QList<TextAutoGenerateText::TextAutoGenerateMessage> messages;
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo1"_ba);
        msg.setContent(u"kde and kde"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo2"_ba);
        msg.setContent(u"bla bla"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo3"_ba);
        msg.setContent(u"kde"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    model.setMessages(messages);
    const int nbSearchString = model.setSearchText(u"kde"_s);
    RuqolaQuickSearchMessageSettings messageSettings(&model);

    QCOMPARE(nbSearchString, 3);
    QSignalSpy spy(&messageSettings, &RuqolaQuickSearchMessageSettings::refreshMessage);

    messageSettings.next();
    QCOMPARE(spy.count(), 1);
    // Last one
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.next();
    // No other message
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);
#endif
}

void RuqolaQuickSearchMessageSettingsTest::shouldTestPreviousSearchStringIndex()
{
#if 0
    TextAutoGenerateText::TextAutoGenerateMessagesModel model;
    QList<TextAutoGenerateText::TextAutoGenerateMessage> messages;
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo1"_ba);
        msg.setContent(u"kde and kde"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo2"_ba);
        msg.setContent(u"bla bla"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    {
        TextAutoGenerateText::TextAutoGenerateMessage msg;
        msg.setUuid("foo3"_ba);
        msg.setContent(u"kde"_s);
        msg.setSender(TextAutoGenerateText::TextAutoGenerateMessage::Sender::User);
        msg.generateHtml();
        messages.append(msg);
    }
    model.setMessages(messages);
    const int nbSearchString = model.setSearchText(u"kde"_s);
    RuqolaQuickSearchMessageSettings messageSettings(&model);

    QSignalSpy spy(&messageSettings, &RuqolaQuickSearchMessageSettings::refreshMessage);

    QCOMPARE(nbSearchString, 3);
    messageSettings.previous();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0), "foo3"_ba);
    QCOMPARE(spy.at(0).at(1), ""_ba);
    QCOMPARE(spy.at(0).at(2), 0);

    // Last one
    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo3"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    messageSettings.previous();

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0), "foo1"_ba);
    QCOMPARE(spy.at(1).at(1), "foo3"_ba);
    QCOMPARE(spy.at(1).at(2), 1);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 1);

    messageSettings.previous();

    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0), "foo1"_ba);
    QCOMPARE(spy.at(2).at(1), "foo1"_ba);
    QCOMPARE(spy.at(2).at(2), 0);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);

    // no other search element
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

    messageSettings.previous();
    // No new signal
    QCOMPARE(spy.count(), 3);

    QCOMPARE(messageSettings.currentMessageIdentifier(), "foo1"_ba);
    QCOMPARE(messageSettings.currentSearchIndex(), 0);
#endif
}

#include "moc_ruqolaquicksearchmessagesettingstest.cpp"
