/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "inputtextmanagertest.h"
#include "inputtextmanager.h"
#include "model/inputcompletermodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(InputTextManagerTest)

InputTextManagerTest::InputTextManagerTest(QObject *parent)
    : QObject(parent)
{
}

void InputTextManagerTest::shouldHaveDefaultValue()
{
    InputTextManager manager(nullptr);
    QVERIFY(manager.inputCompleterModel());
    QCOMPARE(manager.inputCompleterModel()->rowCount(), 0);
    //TODO
}

void InputTextManagerTest::shouldReplaceWord_data()
{
    QTest::addColumn<QString>("newword");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("result");
    QTest::newRow("empty") << QString() << QString() << 5 << QString();
    QTest::newRow("replace1") << QStringLiteral("bla") << QStringLiteral("foo @d") << 5 << QStringLiteral("foo @bla");
    QTest::newRow("replace2") << QStringLiteral("bla") << QStringLiteral("foo @daaaa") << 5 << QStringLiteral("foo @bla");
    QTest::newRow("replace3") << QStringLiteral("bla") << QStringLiteral("@daaaa foo") << 1 << QStringLiteral("@bla foo");
}

void InputTextManagerTest::shouldReplaceWord()
{
    QFETCH(QString, newword);
    QFETCH(QString, text);
    QFETCH(int, position);
    QFETCH(QString, result);

    InputTextManager manager(nullptr);
    QCOMPARE(manager.replaceWord(newword, text, position), result);
}

void InputTextManagerTest::shouldSearchWord_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("result");
    QTest::newRow("empty") << QString() << 5 << QString();
}

void InputTextManagerTest::shouldSearchWord()
{
    QFETCH(QString, text);
    QFETCH(int, position);
    QFETCH(QString, result);

    InputTextManager manager(nullptr);
    QCOMPARE(manager.searchWord(text, position), result);
}
