/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrtest.h"
#include "otr/otr.h"
#include "ruqola_autotest_helper.h"
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
    // TODO
}
