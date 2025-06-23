/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statuscomboboxtest.h"
using namespace Qt::Literals::StringLiterals;

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
    w.setStatus(User::PresenceStatus::Offline);
    QCOMPARE(w.currentIndex(), 3);

    w.setStatus(User::PresenceStatus::Away);
    QCOMPARE(w.currentIndex(), 2);
}

#include "moc_statuscomboboxtest.cpp"
