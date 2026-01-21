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

    QCOMPARE(w.itemData(0).toString(), u"t"_s);
    QCOMPARE(w.itemData(1).toString(), u"T"_s);
    QCOMPARE(w.itemData(2).toString(), u"d"_s);
    QCOMPARE(w.itemData(3).toString(), u"D"_s);
    QCOMPARE(w.itemData(4).toString(), u"f"_s);
    QCOMPARE(w.itemData(5).toString(), u"F"_s);
    QCOMPARE(w.itemData(6).toString(), u"R"_s);
}

#include "moc_timestampinmessagesformatcomboboxtest.cpp"
