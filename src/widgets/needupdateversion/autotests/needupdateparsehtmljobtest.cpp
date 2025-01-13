/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "needupdateparsehtmljobtest.h"
#include "needupdateversion/needupdateparsehtmljob.h"
#include <QTest>

QTEST_MAIN(NeedUpdateParseHtmlJobTest)

NeedUpdateParseHtmlJobTest::NeedUpdateParseHtmlJobTest(QObject *parent)
    : QObject{parent}
{
}

void NeedUpdateParseHtmlJobTest::shouldHaveDefaultValues()
{
    NeedUpdateParseHtmlJob job;
    QVERIFY(!job.canStart());
    QVERIFY(job.url().isEmpty());
    // TODO
}

#include "moc_needupdateparsehtmljobtest.cpp"
