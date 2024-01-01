/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationdesktopdurationpreferencemodeltest.h"
#include "model/notificationdesktopdurationpreferencemodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(NotificationDesktopDurationPreferenceModelTest)

NotificationDesktopDurationPreferenceModelTest::NotificationDesktopDurationPreferenceModelTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationDesktopDurationPreferenceModelTest::shouldHaveDefaultValue()
{
    NotificationDesktopDurationPreferenceModel w;
    QCOMPARE(w.rowCount(), 6);

    // FIXME QCOMPARE(w.currentPreference(), 0);
}

// TODO add autotest about currentStatus changed !

#include "moc_notificationdesktopdurationpreferencemodeltest.cpp"
