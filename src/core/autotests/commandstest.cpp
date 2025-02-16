/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandstest.h"
#include "commands/commands.h"
#include "ruqola_autotest_helper.h"
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

    QTest::addRow("command1") << QStringLiteral("command1") << 0 << 0;
    QTest::addRow("command2") << QStringLiteral("command2") << 25 << 25;
    QTest::addRow("command3") << QStringLiteral("command3") << 3 << 3;
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
        d.setCommandName(QStringLiteral("/slackbridge-import"));
        result.append(std::move(d));
    }
    {
        Command d;
        d.setCommandName(QStringLiteral("/archive"));
        d.setDescription(QStringLiteral("Archive"));
        d.setPermissions({QStringLiteral("archive-room")});
        d.setParams(QStringLiteral("#channel"));
        result.append(std::move(d));
    }
    {
        Command d;
        d.setCommandName(QStringLiteral("/leave"));
        d.setDescription(QStringLiteral("Leave_the_current_channel"));
        d.setPermissions({{QStringLiteral("leave-c")}, {QStringLiteral("leave-p")}});
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
