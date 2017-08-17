/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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


#include "utilstest.h"
#include "utils.h"
#include <QTest>
QTEST_MAIN(UtilsTest)

UtilsTest::UtilsTest(QObject *parent)
    : QObject(parent)
{

}

void UtilsTest::shouldGenerateServerUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QUrl>("output");
    QTest::newRow("empty") << QString() << QUrl();
    QTest::newRow("http") << QStringLiteral("http://foo.kde.org/") << QUrl(QStringLiteral("ws://foo.kde.org//websocket"));
    QTest::newRow("http2") << QStringLiteral("http://foo.kde.org") << QUrl(QStringLiteral("ws://foo.kde.org/websocket"));
    QTest::newRow("https") << QStringLiteral("https://foo.kde.org") << QUrl(QStringLiteral("wss://foo.kde.org/websocket"));
    QTest::newRow("withoutscheme") << QStringLiteral("foo.kde.org") << QUrl(QStringLiteral("wss://foo.kde.org/websocket"));
}

void UtilsTest::shouldGenerateServerUrl()
{
    QFETCH(QString, input);
    QFETCH(QUrl, output);
    QCOMPARE(Utils::generateServerUrl(input), output);
}
