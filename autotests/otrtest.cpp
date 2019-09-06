/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "otrtest.h"
#include "otr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>

QTEST_GUILESS_MAIN(OtrTest)

OtrTest::OtrTest(QObject *parent)
    : QObject(parent)
{
}

void OtrTest::shouldHaveDefaultValue()
{
    Otr t;
    QVERIFY(t.roomId().isEmpty());
    QVERIFY(t.userId().isEmpty());
    QCOMPARE(t.type(), Otr::OtrType::Unknown);
    QVERIFY(!t.isValid());
}

void OtrTest::shouldParseOtr_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<Otr::OtrType>("otrtype");
    QTest::newRow("otrend") << QStringLiteral("otrend") << Otr::End;
    QTest::newRow("otrbegin") << QStringLiteral("otrbegin") << Otr::Handshake;
}

void OtrTest::shouldParseOtr()
{
    QFETCH(QString, fileName);
    QFETCH(Otr::OtrType, otrtype);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + fileName + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object().value(QLatin1String("fields")).toObject();
    const QJsonArray contents = fields.value(QLatin1String("args")).toArray();

    Otr otr;
    otr.parseOtr(contents);
    QVERIFY(otr.isValid());
    QCOMPARE(otr.type(), otrtype);
    //TODO
}
