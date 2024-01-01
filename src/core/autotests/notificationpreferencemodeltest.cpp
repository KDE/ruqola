/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationpreferencemodeltest.h"
#include "model/notificationpreferencemodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(NotificationPreferenceModelTest)

NotificationPreferenceModelTest::NotificationPreferenceModelTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationPreferenceModelTest::shouldHaveDefaultValue()
{
    NotificationPreferenceModel w;
    QCOMPARE(w.rowCount(), 4);

    // FIXME QCOMPARE(w.currentPreference(), 0);
}

// TODO add autotest about currentStatus changed !

#include "moc_notificationpreferencemodeltest.cpp"
