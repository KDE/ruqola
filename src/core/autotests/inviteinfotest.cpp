/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "inviteinfotest.h"
#include "inviteinfo.h"
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
