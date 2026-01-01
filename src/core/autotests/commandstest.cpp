/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandstest.h"
#include "commands/commands.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(CommandsTest)

CommandsTest::CommandsTest(QObject *parent)
    : QObject(parent)
{
}

void CommandsTest::shouldHaveDefaultValue()
{
    Commands r;
    QVERIFY(r.commands().isEmpty());
    QVERIFY(r.isEmpty());
}

void CommandsTest::shouldLoadCommands_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("commandsCount");
    QTest::addColumn<int>("total");

    QTest::addRow("command1") << u"command1"_s << 0 << 0;
    QTest::addRow("command2") << u"command2"_s << 25 << 25;
    QTest::addRow("command3") << u"command3"_s << 3 << 3;
}

void CommandsTest::shouldLoadCommands()
{
    QFETCH(QString, name);
    QFETCH(int, commandsCount);
    QFETCH(int, total);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/commands/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Commands r;
    r.parseCommands(obj);
    QCOMPARE(r.commandsCount(), commandsCount);
    QCOMPARE(r.total(), total);
}

void CommandsTest::shouldLoadPermissions()
{
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/commands/command3.json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Commands r;
    r.parseCommands(obj);
    QCOMPARE(r.commandsCount(), 3);

    QList<Command> result;
    {
        Command d;
        d.setCommandName(u"/slackbridge-import"_s);
        result.append(std::move(d));
    }
    {
        Command d;
        d.setCommandName(u"/archive"_s);
        d.setDescription(u"Archive"_s);
        d.setPermissions({u"archive-room"_s});
        d.setParams(u"#channel"_s);
        result.append(std::move(d));
    }
    {
        Command d;
        d.setCommandName(u"/leave"_s);
        d.setDescription(u"Leave_the_current_channel"_s);
        d.setPermissions({{u"leave-c"_s}, {u"leave-p"_s}});
        result.append(std::move(d));
    }

    const bool equalResult = r.commands() == result;
    if (!equalResult) {
        qDebug() << " Expected " << result;
        qDebug() << " result   " << r.commands();
    }
    QVERIFY(equalResult);
}

#include "moc_commandstest.cpp"
