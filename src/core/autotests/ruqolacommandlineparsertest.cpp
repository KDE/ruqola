/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolacommandlineparsertest.h"
#include "config-ruqola.h"
#include "ruqolacommandlineparser.h"
#include <QTest>
QTEST_GUILESS_MAIN(RuqolaCommandLineParserTest)
using namespace Qt::Literals::StringLiterals;
RuqolaCommandLineParserTest::RuqolaCommandLineParserTest(QObject *parent)
    : QObject{parent}
{
}
void RuqolaCommandLineParserTest::shouldVerifyCommandLineFromEnumValues()
{
    QCOMPARE(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::ListAccount), u"list-accounts"_s);
    QCOMPARE(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::Account), u"account"_s);
    QCOMPARE(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::MessageUrl), u"messageurl"_s);
    QCOMPARE(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::LoginDdpApi), u"loginddpapi"_s);
#if HAVE_KUSERFEEDBACK
    QCOMPARE(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::FeedBack), u"feedback"_s);
#endif
}

#include "moc_ruqolacommandlineparsertest.cpp"
