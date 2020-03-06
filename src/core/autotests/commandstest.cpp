/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "commandstest.h"
#include "commands.h"
#include <QJsonDocument>
#include <QTest>
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

    QTest::addRow("command1") << QStringLiteral("command1") << 0;
    QTest::addRow("command2") << QStringLiteral("command2") << 25;
}

void CommandsTest::shouldLoadCommands()
{
    QFETCH(QString, name);
    QFETCH(int, commandsCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/commands/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();

    Commands r;
    r.parseCommands(obj);
    QCOMPARE(r.commandsCount(), commandsCount);
}
