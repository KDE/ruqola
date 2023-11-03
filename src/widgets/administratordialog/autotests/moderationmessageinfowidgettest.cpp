/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidgettest.h"
#include "administratordialog/moderationconsole/moderationmessageinfowidget.h"
#include "administratordialog/moderationconsole/moderationreportinfowidget.h"
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

    auto mModerationReportInfoWidget = d.findChild<ModerationReportInfoWidget *>(QStringLiteral("mModerationReportInfoWidget"));
    QVERIFY(mModerationReportInfoWidget);
}

#include "moc_moderationmessageinfowidgettest.cpp"
