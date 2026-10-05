/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidgettest.h"

#include "administratordialog/moderationconsole/moderationmessageinfowidget.h"
#include "administratordialog/moderationconsole/moderationreportinfowidget.h"
#include <QTest>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(ModerationMessageInfoWidgetTest)
ModerationMessageInfoWidgetTest::ModerationMessageInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationMessageInfoWidgetTest::shouldHaveDefaultValues()
{
    const ModerationMessageInfoWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mModerationReportInfoWidget = d.findChild<ModerationReportInfoWidget *>(u"mModerationReportInfoWidget"_s);
    QVERIFY(mModerationReportInfoWidget);
}

#include "moc_moderationmessageinfowidgettest.cpp"
