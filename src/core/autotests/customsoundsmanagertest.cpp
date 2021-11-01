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

#include "customsoundsmanagertest.h"
#include "customsound/customsoundsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(CustomSoundsManagerTest)

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
    {
        QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("deleteCustomSounds1") << 5 << 4 << 1;
    }
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
    {
        QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("addCustomSounds1") << 5 << 6 << 1;
    }
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
    {
        QTest::addRow("customSounds1") << QStringLiteral("customSounds1") << QStringLiteral("updateCustomSounds1") << 5 << 5 << 1;
    }
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
