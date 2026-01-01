/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inputtextmanagertest.h"
using namespace Qt::Literals::StringLiterals;

#include "inputtextmanager.h"
#include "model/inputcompletermodel.h"
#include "rocketchataccount.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(InputTextManagerTest)

InputTextManagerTest::InputTextManagerTest(QObject *parent)
    : QObject(parent)
{
}

void InputTextManagerTest::shouldHaveDefaultValue()
{
    InputTextManager manager(nullptr, nullptr);
    QVERIFY(manager.inputCompleterModel());
    QCOMPARE(manager.inputCompleterModel()->rowCount(), 0);
}

void InputTextManagerTest::shouldReplaceWord_data()
{
    QTest::addColumn<QString>("newword");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("result");
    QTest::addColumn<int>("expectedPosition");
    QTest::newRow("empty") << QString() << QString() << 5 << QString() << 5;
    QTest::newRow("replace1") << u"bla"_s << u"foo @d"_s << 5 << u"foo @bla"_s << 8;
    QTest::newRow("replace2") << u"bla"_s << u"foo @daaaa"_s << 5 << u"foo @bla"_s << 8;
    QTest::newRow("replace3") << u"bla"_s << u"@daaaa foo"_s << 1 << u"@bla foo"_s << 4;
    QTest::newRow("buildbot") << u"buildbot "_s << u"@bu"_s << 3 << u"@buildbot "_s << 10;
    QTest::newRow("trailing_space_already_there") << u"bla "_s << u"@daaaa foo"_s << 1 << u"@bla foo"_s << 5;
}

void InputTextManagerTest::shouldReplaceWord()
{
    QFETCH(QString, newword);
    QFETCH(QString, text);
    QFETCH(int, position);
    QFETCH(int, expectedPosition);
    QFETCH(QString, result);

    InputTextManager manager(nullptr, nullptr);

    // Widgets
    QCOMPARE(manager.applyCompletion(newword, text, &position), result);
    QCOMPARE(position, expectedPosition);
}

void InputTextManagerTest::shouldSearchWord_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("result");
    QTest::newRow("empty") << QString() << 5 << QString();
    QTest::newRow("one_letter") << u"l"_s << 1 << u"l"_s;
    QTest::newRow("at") << u"@"_s << 1 << u"@"_s;
    QTest::newRow("at_letter_1") << u"@a"_s << 1 << u"@a"_s;
    QTest::newRow("at_letter") << u"@a"_s << 2 << u"@a"_s;
    QTest::newRow("at_word_space") << u"@abc "_s << 5 << QString();
}

void InputTextManagerTest::shouldSearchWord()
{
    QFETCH(QString, text);
    QFETCH(int, position);
    QFETCH(QString, result);

    InputTextManager manager(nullptr, nullptr);
    int start; // TODO test the output value
    QCOMPARE(manager.searchWord(text, position, start), result);
}

void InputTextManagerTest::shouldEmitCompletionRequestSignals()
{
    RocketChatAccount account;
    InputTextManager manager(&account, nullptr);
    QSignalSpy typeChangedSpy(&manager, &InputTextManager::completionTypeChanged);
    QSignalSpy requestSpy(&manager, &InputTextManager::completionRequested);

    manager.setInputTextChanged(QByteArray(), u"a @"_s, 3);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::User);
    QCOMPARE(requestSpy.count(), 0); // We don't send signal when we have only "@" now.
    typeChangedSpy.clear();
    requestSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"a :"_s, 3);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::Emoji);
    typeChangedSpy.clear();
    QCOMPARE(requestSpy.count(), 0); // emoji completion doesn't use this signal
    requestSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"a "_s, 2);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::None);
    typeChangedSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"a #c"_s, 4);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::Channel);
    QCOMPARE(requestSpy.count(), 1);
    QCOMPARE(requestSpy.at(0).at(1).toString(), u"c"_s);
    typeChangedSpy.clear();
    requestSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"hello @foo"_s, 10);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::User);
    QCOMPARE(requestSpy.count(), 1);
    QCOMPARE(requestSpy.at(0).at(1).toString(), u"foo"_s);
    requestSpy.clear();
    typeChangedSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"@foo hello"_s, 4);
    QCOMPARE(typeChangedSpy.count(), 0); // User again
    QCOMPARE(requestSpy.count(), 1);
    QCOMPARE(requestSpy.at(0).at(1).toString(), u"foo"_s);
    requestSpy.clear();
    typeChangedSpy.clear();

    manager.setInputTextChanged(QByteArray(), u"a :heart:"_s, 9);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::Emoji);
    QCOMPARE(requestSpy.count(), 0);
    typeChangedSpy.clear();

    // If the cursor isn't at the end of the word, don't trigger completion
    manager.setInputTextChanged(QByteArray(), u"@foo"_s, 2);
    QCOMPARE(typeChangedSpy.count(), 1);
    QCOMPARE(typeChangedSpy.at(0).at(0).value<InputTextManager::CompletionForType>(), InputTextManager::CompletionForType::None);
    QCOMPARE(requestSpy.count(), 0);
    typeChangedSpy.clear();
}

#include "moc_inputtextmanagertest.cpp"
