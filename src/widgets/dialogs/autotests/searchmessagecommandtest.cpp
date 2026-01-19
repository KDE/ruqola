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

#include "moc_searchmessagecommandtest.cpp"
