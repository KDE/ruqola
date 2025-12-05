/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "videoconferenceinfostest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconferenceinfos.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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

void VideoConferenceInfosTest::shouldLoadVideoConferenceInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("videoConferenceInfosCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<int>("offset");

    QTest::addRow("empty") << u"videoconferenceinfos-empty"_s << 0 << 0 << 0;
    QTest::addRow("1-element") << u"videoconferenceinfos-1-element"_s << 1 << 1 << 0;
}

void VideoConferenceInfosTest::shouldLoadVideoConferenceInfos()
{
    QFETCH(QString, name);
    QFETCH(int, videoConferenceInfosCount);
    QFETCH(int, total);
    QFETCH(int, offset);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/videoconferenceinfos/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    VideoConferenceInfos m;
    m.parseVideoConferenceInfos(obj);
    QCOMPARE(m.videoConferenceInfosCount(), videoConferenceInfosCount);
    QCOMPARE(m.total(), total);
    QCOMPARE(m.offset(), offset);
}

#include "moc_videoconferenceinfostest.cpp"
