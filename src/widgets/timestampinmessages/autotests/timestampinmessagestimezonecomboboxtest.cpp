/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagestimezonecomboboxtest.h"
#include "timestampinmessages/timestampinmessagestimezonecombobox.h"
#include <QTest>

QTEST_MAIN(TimeStampInMessagesTimeZoneComboBoxTest)

TimeStampInMessagesTimeZoneComboBoxTest::TimeStampInMessagesTimeZoneComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesTimeZoneComboBoxTest::shouldHaveDefaultValues()
{
    const TimeStampInMessagesTimeZoneComboBox w;
    QVERIFY(w.count() > 0);
}

#include "moc_timestampinmessagestimezonecomboboxtest.cpp"
