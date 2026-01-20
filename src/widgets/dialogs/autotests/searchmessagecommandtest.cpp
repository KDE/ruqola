/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagecommandtest.h"
#include "dialogs/searchmessagecommand.h"
#include <QTest>
QTEST_GUILESS_MAIN(SearchMessageCommandTest)
using namespace Qt::Literals::StringLiterals;
SearchMessageCommandTest::SearchMessageCommandTest(QObject *parent)
    : QObject{parent}
{
}

void SearchMessageCommandTest::shouldGenerateText()
{
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::Unknown), QString{});
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::HasStar), u"has:star"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::IsPinned), u"is:pinned"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::HasUrl), u"has:url"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::HasLocation), u"has:location"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::Before), u"before:"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::After), u"after:"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::Day), u"on:"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::Order), u"order:desc"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::FromMe), u"from:me"_s);
    QCOMPARE(SearchMessageCommand::generateCommandText(SearchMessageCommand::SearchMessageCommandType::FromUserName), u"from:user.name"_s);
}

void SearchMessageCommandTest::shouldVerifyNeedUnique()
{
    QVERIFY(!SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::Unknown));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::HasStar));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::IsPinned));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::HasUrl));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::HasLocation));
    QVERIFY(!SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::Before));
    QVERIFY(!SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::After));
    QVERIFY(!SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::Day));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::Order));
    QVERIFY(SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::FromMe));
    QVERIFY(!SearchMessageCommand::mustBeUnique(SearchMessageCommand::SearchMessageCommandType::FromUserName));
}

#include "moc_searchmessagecommandtest.cpp"
