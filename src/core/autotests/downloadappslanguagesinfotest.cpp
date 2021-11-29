/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "downloadappslanguagesinfotest.h"
#include "downloadappslanguages/downloadappslanguagesinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(DownloadAppsLanguagesInfoTest)
DownloadAppsLanguagesInfoTest::DownloadAppsLanguagesInfoTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAppsLanguagesInfoTest::shouldHaveDefaultValues()
{
    DownloadAppsLanguagesInfo info;
    QVERIFY(info.languageMap().isEmpty());
    QVERIFY(info.isEmpty());
}
