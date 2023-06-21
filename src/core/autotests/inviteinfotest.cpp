/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteinfotest.h"
#include "invite/inviteinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(InviteInfoTest)

InviteInfoTest::InviteInfoTest(QObject *parent)
    : QObject(parent)
{
}

void InviteInfoTest::shouldHaveDefaultValues()
{
    InviteInfo w;
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(w.userIdentifier().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QCOMPARE(w.maxUses(), 0);
    QCOMPARE(w.uses(), 0);
    QVERIFY(!w.expireDateTime().isValid());
    QVERIFY(!w.createDateTime().isValid());
}

void InviteInfoTest::shouldParseInviteInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<InviteInfo>("expectedInvite");
    {
        // RestAPI
        InviteInfo expected;

        expected.setIdentifier(QStringLiteral("D2F6of"));
        expected.setMaxUses(25);
        expected.setUserIdentifier(QStringLiteral("H7Q9djXQ4iShzD9T2"));
        expected.setRoomId(QStringLiteral("n2GWePY4zjG48g7qA"));
        expected.setUses(0);

        QDateTime expireDateTime;
        expireDateTime.setDate(QDate(2021, 04, 8));
        expireDateTime.setTime(QTime(8, 49, 04, 571));
        expireDateTime.setTimeSpec(Qt::LocalTime);
        expected.setExpireDateTime(expireDateTime);

        QDateTime createDateTime;
        createDateTime.setDate(QDate(2021, 04, 7));
        createDateTime.setTime(QTime(8, 49, 04, 571));
        createDateTime.setTimeSpec(Qt::LocalTime);
        expected.setCreateDateTime(createDateTime);
        QTest::newRow("inviteinfo") << QStringLiteral("inviteinfo") << expected;
    }
}

void InviteInfoTest::shouldParseInviteInfo()
{
    QFETCH(QString, fileName);
    QFETCH(InviteInfo, expectedInvite);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + fileName + QLatin1String(".json");
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    InviteInfo newFile;
    newFile.parseInviteInfo(fields);
    const bool equal = (newFile == expectedInvite);
    if (!equal) {
        qDebug() << " current value " << newFile;
        qDebug() << " expected value " << expectedInvite;
    }
    QVERIFY(equal);
}

#include "moc_inviteinfotest.cpp"
