/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesformatcomboboxtest.h"
#include "timestampinmessages/timestampinmessagesformatcombobox.h"
#include <QTest>
QTEST_MAIN(TimeStampInMessagesFormatComboBoxTest)
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesFormatComboBoxTest::TimeStampInMessagesFormatComboBoxTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesFormatComboBoxTest::shouldHaveDefaultValues()
{
    const TimeStampInMessagesFormatComboBox w;
    QVERIFY(w.count() > 0);
    QCOMPARE(w.count(), 7);

    QCOMPARE(w.itemData(0), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortTime));
    QCOMPARE(w.itemData(1), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongTime));
    QCOMPARE(w.itemData(2), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::ShortDate));
    QCOMPARE(w.itemData(3), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongDate));
    QCOMPARE(w.itemData(4), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::FullDateTime));
    QCOMPARE(w.itemData(5), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::LongFullDateTime));
    QCOMPARE(w.itemData(6), QVariant::fromValue(TimeStampInMessagesUtils::FormatType::RelativeTime));
}

#include "moc_timestampinmessagesformatcomboboxtest.cpp"
