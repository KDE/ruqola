/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QVERIFY(notifier.accountName().isEmpty());
}
