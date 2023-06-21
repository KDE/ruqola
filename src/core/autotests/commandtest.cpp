/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandtest.h"
#include "commands/command.h"
#include <QTest>
QTEST_GUILESS_MAIN(CommandTest)

CommandTest::CommandTest(QObject *parent)
    : QObject(parent)
{
}

void CommandTest::shouldHaveDefaultValues()
{
    Command c;
    QVERIFY(c.params().isEmpty());
    QVERIFY(c.permissions().isEmpty());
    QVERIFY(c.commandName().isEmpty());
    QVERIFY(c.description().isEmpty());
    QVERIFY(!c.clientOnly());
    QVERIFY(!c.providesPreview());
}

#include "moc_commandtest.cpp"
