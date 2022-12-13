/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencetest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconference.h"
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceTest)
VideoConferenceTest::VideoConferenceTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceTest::shouldHaveDefaultValues()
{
    VideoConference w;
    QVERIFY(w.callId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.userId().isEmpty());
    QVERIFY(!w.isValid());
}

void VideoConferenceTest::shouldExtractVideoConference()
{
    QFETCH(QString, name);
    QFETCH(VideoConference, videoConference);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/VideoConference/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    VideoConference result;
    result.parseVideoConference(obj);
    const bool equal = result == videoConference;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " deviceInfo " << videoConference;
    }
    QVERIFY(equal);
}

void VideoConferenceTest::shouldExtractVideoConference_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<VideoConference>("videoConference");
    QTest::addRow("deviceinfoempty") << QStringLiteral("deviceinfoempty") << VideoConference();
}
