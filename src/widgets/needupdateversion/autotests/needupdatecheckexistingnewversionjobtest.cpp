/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdatecheckexistingnewversionjobtest.h"
#include "needupdateversion/needupdatecheckexistingnewversionjob.h"
#include <QTest>

QTEST_MAIN(NeedUpdateCheckExistingNewVersionJobTest)

NeedUpdateCheckExistingNewVersionJobTest::NeedUpdateCheckExistingNewVersionJobTest(QObject *parent)
    : QObject{parent}
{
}

void NeedUpdateCheckExistingNewVersionJobTest::shouldHaveDefaultValues()
{
    NeedUpdateCheckExistingNewVersionJob w;
    QVERIFY(!w.canStart());
    QVERIFY(w.url().isEmpty());
}

#include "moc_needupdatecheckexistingnewversionjobtest.cpp"
