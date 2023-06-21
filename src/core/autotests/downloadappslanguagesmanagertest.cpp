/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadappslanguagesmanagertest.h"
#include "downloadappslanguages/downloadappslanguagesmanager.h"
#include <QTest>

QTEST_GUILESS_MAIN(DownloadAppsLanguagesManagerTest)
DownloadAppsLanguagesManagerTest::DownloadAppsLanguagesManagerTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAppsLanguagesManagerTest::shouldHaveDefaultValues()
{
    DownloadAppsLanguagesManager m;
    QVERIFY(!m.fileParsed());
    QVERIFY(m.accountName().isEmpty());
    QVERIFY(m.serverVersion().isEmpty());
}

#include "moc_downloadappslanguagesmanagertest.cpp"
