/*
   Copyright (C) 2018 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "filetest.h"
#include "file.h"

#include <QTest>

QTEST_GUILESS_MAIN(FileTest)

FileTest::FileTest(QObject *parent)
    : QObject(parent)
{

}

void FileTest::shouldHaveDefaultValue()
{
    File f;
    QVERIFY(f.name().isEmpty());
    QVERIFY(f.description().isEmpty());
    QVERIFY(f.userId().isEmpty());
    QVERIFY(f.url().isEmpty());
}

void FileTest::shouldAssignValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QString userId = QStringLiteral("ble");
    f.setUrl(url);
    f.setName(name);
    f.setDescription(description);
    f.setUserId(userId);

    QCOMPARE(f.url(), url);
    QCOMPARE(f.name(), name);
    QCOMPARE(f.description(), description);
    QCOMPARE(f.userId(), userId);
}

void FileTest::shouldCopyValue()
{
    File f;
    const QString url = QStringLiteral("foo");
    const QString name = QStringLiteral("bla");
    const QString description = QStringLiteral("des");
    const QString userId = QStringLiteral("ble");
    f.setUrl(url);
    f.setName(name);
    f.setDescription(description);
    f.setUserId(userId);

    File f2 = f;
    QCOMPARE(f2, f);
}
