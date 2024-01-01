/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesjobtest.h"
#include "downloadappslanguages/downloadappslanguagesjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(DownloadAppsLanguagesJobTest)
DownloadAppsLanguagesJobTest::DownloadAppsLanguagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAppsLanguagesJobTest::shouldHaveDefaultValues()
{
    DownloadAppsLanguagesJob job;
    QVERIFY(!job.canStart());
    QVERIFY(job.serverUrl().isEmpty());
}

#include "moc_downloadappslanguagesjobtest.cpp"
