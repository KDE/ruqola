/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "videoconferenceinfostest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconferenceinfos.h"
#include <QJsonObject>

QTEST_GUILESS_MAIN(VideoConferenceInfosTest)

VideoConferenceInfosTest::VideoConferenceInfosTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConferenceInfosTest::shouldHaveDefaultValues()
{
    VideoConferenceInfos w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.videoConferenceInfosCount(), 0);
}

void VideoConferenceInfosTest::shouldLoadDeviceInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("deviceInfosCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << QStringLiteral("deviceinfos-empty") << 0 << 0 << 0;
    QTest::addRow("1-element") << QStringLiteral("deviceinfos-1-element") << 50 << 2 << 0;
}

void VideoConferenceInfosTest::shouldLoadDeviceInfos()
{
    QFETCH(QString, name);
    QFETCH(int, deviceInfosCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/videoconferenceinfos/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    VideoConferenceInfos m;
    m.parseVideoConferenceInfos(obj);
    QCOMPARE(m.videoConferenceInfosCount(), deviceInfosCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
}

#include "moc_videoconferenceinfostest.cpp"
