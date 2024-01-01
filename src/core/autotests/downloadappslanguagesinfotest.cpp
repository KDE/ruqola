/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_downloadappslanguagesinfotest.cpp"
