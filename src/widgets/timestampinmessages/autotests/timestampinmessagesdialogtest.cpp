/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesdialogtest.h"
#include "timestampinmessages/timestampinmessagesdialog.h"
#include "timestampinmessages/timestampinmessageswidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TimeStampInMessagesDialogTest)
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesDialogTest::TimeStampInMessagesDialogTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesDialogTest::shouldHaveDefaultValues()
{
    const TimeStampInMessagesDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mTimeStampInMessagesWidget = d.findChild<TimeStampInMessagesWidget *>(u"mTimeStampInMessagesWidget"_s);
    QVERIFY(mTimeStampInMessagesWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), {QDialogButtonBox::Ok | QDialogButtonBox::Cancel});
}

#include "moc_timestampinmessagesdialogtest.cpp"
