/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationreportinfolistviewtest.h"
#include "administratordialog/moderationconsole/moderationreportinfolistview.h"
#include <QTest>
QTEST_MAIN(ModerationReportInfoListViewTest)

ModerationReportInfoListViewTest::ModerationReportInfoListViewTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportInfoListViewTest::shouldHaveDefaultValues()
{
    ModerationReportInfoListView w(nullptr);
    QVERIFY(w.searchText().isEmpty());
}

#include "moc_moderationreportinfolistviewtest.cpp"
