/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrtest.h"
#include "otr/otr.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    QTest::newRow("otrend") << u"otrend"_s << Otr::OtrType::End;
    QTest::newRow("otrbegin") << u"otrbegin"_s << Otr::OtrType::Handshake;
}

void OtrTest::shouldParseOtr()
{
    QFETCH(QString, fileName);
    QFETCH(Otr::OtrType, otrtype);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + fileName + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object().value("fields"_L1).toObject();
    const QJsonArray contents = fields.value("args"_L1).toArray();

    Otr otr;
    otr.parseOtr(contents);
    QVERIFY(otr.isValid());
    QCOMPARE(otr.type(), otrtype);
    // TODO
}

#include "moc_otrtest.cpp"
