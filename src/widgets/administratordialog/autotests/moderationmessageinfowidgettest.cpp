/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidgettest.h"
#include "administratordialog/moderationconsole/moderationmessageinfowidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ModerationMessageInfoWidgetTest)
ModerationMessageInfoWidgetTest::ModerationMessageInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationMessageInfoWidgetTest::shouldHaveDefaultValues()
{
    ModerationMessageInfoWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mReportInfoText = d.findChild<QLabel *>(QStringLiteral("mReportInfoText"));
    QVERIFY(mReportInfoText);
    QVERIFY(mReportInfoText->text().isEmpty());
    QVERIFY(mReportInfoText->wordWrap());
}

#include "moc_moderationmessageinfowidgettest.cpp"
