/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsmanagertest.h"
#include "customsound/customsoundsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QSignalSpy>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(CustomSoundsManagerTest)

CustomSoundsManagerTest::CustomSoundsManagerTest(QObject *parent)
    : QObject{parent}
{
}

void CustomSoundsManagerTest::shouldHaveDefaultValues()
{
    CustomSoundsManager w;
    QVERIFY(w.customSoundsInfo().isEmpty());
}

void CustomSoundsManagerTest::shouldDeleteCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, deleteFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + name + ".json"_L1;
    const QJsonArray customSoundsArray = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    CustomSoundsManager w;
    QSignalSpy spyDeleteSignal(&w, &CustomSoundsManager::customSoundRemoved);
    w.parseCustomSounds(customSoundsArray);

    //    CustomEmoji originalEmoji;
    //    originalEmoji.parseEmoji(obj);
    //    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    //    if (!emojiIsEqual) {
    //        qDebug() << "originalEmoji " << originalEmoji;
    //        qDebug() << "ExpectedEmoji " << expectedEmoji;
    //    }
    QCOMPARE(w.customSoundsInfo().count(), initialNumberOfSounds);

    QString deleteJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + deleteFileName + ".json"_L1;
    const QJsonArray deleteCustomSoundsArray = AutoTestHelper::loadJsonArrayObject(deleteJsonFile);
    w.deleteCustomSounds(deleteCustomSoundsArray);
    QCOMPARE(w.customSoundsInfo().count(), afterDeletingNumberOfSounds);

    QCOMPARE(spyDeleteSignal.count(), signalsEmittingCount);
}

void CustomSoundsManagerTest::shouldDeleteCustomSounds_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("deleteFileName");
    QTest::addColumn<int>("initialNumberOfSounds");
    QTest::addColumn<int>("afterDeletingNumberOfSounds");
    QTest::addColumn<int>("signalsEmittingCount");
    QTest::addRow("customSounds1") << u"customSounds1"_s << u"deleteCustomSounds1"_s << 22 << 21 << 1;
    QTest::addRow("customSounds1") << u"customSounds1"_s << u"deleteCustomSounds2"_s << 22 << 22 << 0;
}

void CustomSoundsManagerTest::shouldAddCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, addFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + name + ".json"_L1;
    const QJsonArray customSoundsArray = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    CustomSoundsManager w;
    QSignalSpy spyAddSignal(&w, &CustomSoundsManager::customSoundAdded);
    w.parseCustomSounds(customSoundsArray);

    //    CustomEmoji originalEmoji;
    //    originalEmoji.parseEmoji(obj);
    //    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    //    if (!emojiIsEqual) {
    //        qDebug() << "originalEmoji " << originalEmoji;
    //        qDebug() << "ExpectedEmoji " << expectedEmoji;
    //    }
    QCOMPARE(w.customSoundsInfo().count(), initialNumberOfSounds);

    QString addJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + addFileName + ".json"_L1;
    const QJsonArray addCustomSoundsArray = AutoTestHelper::loadJsonArrayObject(addJsonFile);
    w.updateCustomSounds(addCustomSoundsArray);
    QCOMPARE(w.customSoundsInfo().count(), afterDeletingNumberOfSounds);

    QCOMPARE(spyAddSignal.count(), signalsEmittingCount);
}

void CustomSoundsManagerTest::shouldAddCustomSounds_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("addFileName");
    QTest::addColumn<int>("initialNumberOfSounds");
    QTest::addColumn<int>("afterDeletingNumberOfSounds");
    QTest::addColumn<int>("signalsEmittingCount");
    QTest::addRow("customSounds1") << u"customSounds1"_s << u"addCustomSounds1"_s << 22 << 23 << 1;
}

void CustomSoundsManagerTest::shouldUpdateCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, updateFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + name + ".json"_L1;
    const QJsonArray customSoundsArray = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    CustomSoundsManager w;
    QSignalSpy spyUpdateSignal(&w, &CustomSoundsManager::customSoundUpdated);
    w.parseCustomSounds(customSoundsArray);

    //    CustomEmoji originalEmoji;
    //    originalEmoji.parseEmoji(obj);
    //    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    //    if (!emojiIsEqual) {
    //        qDebug() << "originalEmoji " << originalEmoji;
    //        qDebug() << "ExpectedEmoji " << expectedEmoji;
    //    }
    QCOMPARE(w.customSoundsInfo().count(), initialNumberOfSounds);

    QString updateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + updateFileName + ".json"_L1;
    const QJsonArray addCustomSoundsArray = AutoTestHelper::loadJsonArrayObject(updateJsonFile);
    w.updateCustomSounds(addCustomSoundsArray);
    QCOMPARE(w.customSoundsInfo().count(), afterDeletingNumberOfSounds);

    QCOMPARE(spyUpdateSignal.count(), signalsEmittingCount);
}

void CustomSoundsManagerTest::shouldUpdateCustomSounds_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("updateFileName");
    QTest::addColumn<int>("initialNumberOfSounds");
    QTest::addColumn<int>("afterDeletingNumberOfSounds");
    QTest::addColumn<int>("signalsEmittingCount");
    QTest::addRow("customSounds1") << u"customSounds1"_s << u"updateCustomSounds1"_s << 22 << 22 << 1;
}

void CustomSoundsManagerTest::shouldParseCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(int, numberOfSounds);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/customsounds/"_L1 + name + ".json"_L1;
    const QJsonArray customSoundsArray = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    CustomSoundsManager w;
    w.parseCustomSounds(customSoundsArray);
    //    CustomEmoji originalEmoji;
    //    originalEmoji.parseEmoji(obj);
    //    const bool emojiIsEqual = (originalEmoji == expectedEmoji);
    //    if (!emojiIsEqual) {
    //        qDebug() << "originalEmoji " << originalEmoji;
    //        qDebug() << "ExpectedEmoji " << expectedEmoji;
    //    }
    QCOMPARE(w.customSoundsInfo().count(), numberOfSounds);
}

void CustomSoundsManagerTest::shouldParseCustomSounds_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("numberOfSounds");
    {
        QTest::addRow("customSounds1") << u"customSounds1"_s << 22;
    }
}

#include "moc_customsoundsmanagertest.cpp"
