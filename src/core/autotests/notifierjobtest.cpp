/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifierjobtest.h"
#include "notifications/notifierjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotifierJobTest)

NotifierJobTest::NotifierJobTest(QObject *parent)
    : QObject(parent)
{
}

void NotifierJobTest::shouldHaveDefaultValues()
{
    NotifierJob notifier;
    QVERIFY(!notifier.info().isValid());
}
