/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopsoundpreferencemodeltest.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(NotificationDesktopSoundPreferenceModelTest)

NotificationDesktopSoundPreferenceModelTest::NotificationDesktopSoundPreferenceModelTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationDesktopSoundPreferenceModelTest::shouldHaveDefaultValue()
{
    NotificationDesktopSoundPreferenceModel w;
    // QCOMPARE(w.rowCount(), 15);
    QVERIFY(!w.customSoundManager());
}

#include "moc_notificationdesktopsoundpreferencemodeltest.cpp"
