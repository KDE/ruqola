/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatetextjobtest.h"
#include "translatetext/translatetextjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(TranslateTextJobTest)

TranslateTextJobTest::TranslateTextJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateTextJobTest::shouldHaveDefaultValues()
{
    TranslateTextJob job;
    QVERIFY(!job.info().isValid());
}

#include "moc_translatetextjobtest.cpp"
