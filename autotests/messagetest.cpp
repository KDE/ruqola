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

#include "messagetest.h"
#include "message.h"
#include <QTest>
#include <QJsonDocument>
QTEST_MAIN(MessageTest)
MessageTest::MessageTest(QObject *parent)
    : QObject(parent)
{

}

void MessageTest::shouldParseMessage_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Message>("expectedMessage");
    QTest::addRow("first") << QStringLiteral("first") << Message();
}

void MessageTest::shouldParseMessage()
{
    QFETCH(QString, name);
    QFETCH(Message, expectedMessage);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject obj = doc.object();
    Message originalMessage;
    originalMessage.parseMessage(obj);
    qDebug() << " originalMessage"<<originalMessage;
    QCOMPARE(originalMessage, expectedMessage);
}

