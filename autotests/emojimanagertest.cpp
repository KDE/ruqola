/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "emojimanagertest.h"
#include "emojimanager.h"
#include "emoji.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(EmojiManagerTest)

EmojiManagerTest::EmojiManagerTest(QObject *parent)
    : QObject(parent)
{

}

void EmojiManagerTest::shouldParseEmoji_data()
{
    QTest::addColumn<QString>("name");
    QTest::addRow("emoji1") << QStringLiteral("emoji");
}

void EmojiManagerTest::shouldParseEmoji()
{
    QFETCH(QString, name);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();
    EmojiManager manager;
    manager.loadEmoji(obj);
//    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
//    if (!emojiIsEqual) {
//        qDebug() << "originalEmoji " << originalEmoji;
//        qDebug() << "ExpectedEmoji " << expectedEmoji;
//    }
//    QVERIFY(emojiIsEqual);
}
