/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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


#include "channelcounterinfotest.h"
#include "channelcounterinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelCounterInfoTest)
ChannelCounterInfoTest::ChannelCounterInfoTest(QObject *parent)
    : QObject(parent)
{

}

void ChannelCounterInfoTest::shouldHaveDefaultValues()
{
    ChannelCounterInfo info;
    QCOMPARE(info.unreadMessages(), 0);
    QCOMPARE(info.messageCount(), 0);
    QVERIFY(!info.joined());
    QVERIFY(!info.isValid());
}

void ChannelCounterInfoTest::shouldLoadChannelCounter_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ChannelCounterInfo>("channelcounter");
    ChannelCounterInfo result;
    result.setMessageCount(48013);
    result.setUnreadMessages(0);
    result.setJoined(true);
    //TODO
    QTest::addRow("test1") << QStringLiteral("test1") << result;
}

void ChannelCounterInfoTest::shouldLoadChannelCounter()
{
    QFETCH(QString, name);
    QFETCH(ChannelCounterInfo, channelcounter);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/channelcounter/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ChannelCounterInfo r;
    r.parseCounterInfo(obj);
    const bool equalOwner = (r == channelcounter);
    if (!equalOwner) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << channelcounter;
    }
    QVERIFY(equalOwner);
}
