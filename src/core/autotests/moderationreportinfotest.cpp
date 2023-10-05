/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfotest.h"
#include "moderation/moderationreportinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ModerationReportInfoTest)
ModerationReportInfoTest::ModerationReportInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportInfoTest::shouldHaveDefaultValues()
{
    ModerationReportInfo w;
    QVERIFY(w.reportIdentifier().isEmpty());
    QVERIFY(w.description().isEmpty());
}

#include "moc_moderationreportinfotest.cpp"
