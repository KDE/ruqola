/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsmanagertest.h"
#include "customsound/customsoundsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QSignalSpy>

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

void CustomSoundsManagerTest::shouldParseCustomSounds_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("numberOfSounds");
    {
        QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << 5;
    }
}

void CustomSoundsManagerTest::shouldDeleteCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, deleteFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + name + QLatin1String(".json");
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

    QString deleteJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + deleteFileName + QLatin1String(".json");
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
    QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("deleteCustomSounds1") << 5 << 4 << 1;
    QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("deleteCustomSounds2") << 5 << 5 << 0;
}

void CustomSoundsManagerTest::shouldAddCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, addFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + name + QLatin1String(".json");
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

    QString addJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + addFileName + QLatin1String(".json");
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
    QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("addCustomSounds1") << 5 << 6 << 1;
}

void CustomSoundsManagerTest::shouldUpdateCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(QString, updateFileName);
    QFETCH(int, initialNumberOfSounds);
    QFETCH(int, afterDeletingNumberOfSounds);
    QFETCH(int, signalsEmittingCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + name + QLatin1String(".json");
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

    QString updateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + updateFileName + QLatin1String(".json");
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
    QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("updateCustomSounds1") << 5 << 5 << 1;
}

void CustomSoundsManagerTest::shouldParseCustomSounds()
{
    QFETCH(QString, name);
    QFETCH(int, numberOfSounds);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/customsounds/") + name + QLatin1String(".json");
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

#include "moc_customsoundsmanagertest.cpp"
