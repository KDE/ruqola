/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "statuscomboboxtest.h"
#include "misc/statuscombobox.h"
#include <QTest>
QTEST_MAIN(StatusComboboxTest)
StatusComboboxTest::StatusComboboxTest(QObject *parent)
    : QObject(parent)
{
}

void StatusComboboxTest::shouldHaveDefaultValues()
{
    StatusCombobox w;
    w.setUseOnlyStandardStatus();
    QCOMPARE(w.count(), 4);
}

void StatusComboboxTest::shouldSelectStatus()
{
    StatusCombobox w;
    w.setUseOnlyStandardStatus();
    w.setStatus(User::PresenceStatus::PresenceOffline);
    QCOMPARE(w.currentIndex(), 3);

    w.setStatus(User::PresenceStatus::PresenceAway);
    QCOMPARE(w.currentIndex(), 2);
}
