/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandsmodeltest.h"
#include "model/commandsmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(CommandsModelTest)

CommandsModelTest::CommandsModelTest(QObject *parent)
    : QObject(parent)
{
}

void CommandsModelTest::shouldHaveDefaultValues()
{
    CommandsModel d;
    QCOMPARE(d.rowCount(), 0);
}

#include "moc_commandsmodeltest.cpp"
