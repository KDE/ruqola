/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessageswidgettest.h"
#include "timestampinmessages/timestampinmessagesformatcombobox.h"
#include "timestampinmessages/timestampinmessageswidget.h"
#include <QDateEdit>
#include <QFormLayout>
#include <QTest>
#include <QTimeEdit>
QTEST_MAIN(TimeStampInMessagesWidgetTest)
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesWidgetTest::TimeStampInMessagesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesWidgetTest::shouldHaveDefaultValues()
{
    const TimeStampInMessagesWidget d(nullptr);
    auto mainLayout = d.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mDateEdit = d.findChild<QDateEdit *>(u"mDateEdit"_s);
    QVERIFY(mDateEdit);

    auto mTimeEdit = d.findChild<QTimeEdit *>(u"mTimeEdit"_s);
    QVERIFY(mTimeEdit);

    auto mTimeStampInMessagesFormatComboBox = d.findChild<TimeStampInMessagesFormatComboBox *>(u"mTimeStampInMessagesFormatComboBox"_s);
    QVERIFY(mTimeStampInMessagesFormatComboBox);
}

void TimeStampInMessagesWidgetTest::shouldHaveTimeStampInfoDefaultValues()
{
    const TimeStampInMessagesWidget::TimeStampInfo info;
    // TODO
}

#include "moc_timestampinmessageswidgettest.cpp"
