/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "downloadavartarurlmanagertest.h"
#include "downloadavartarurlmanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(DownloadAvartarUrlManagerTest)
DownloadAvartarUrlManagerTest::DownloadAvartarUrlManagerTest(QObject *parent)
    : QObject(parent)
{
}

void DownloadAvartarUrlManagerTest::shouldHaveDefaultValues()
{
    DownloadAvartarUrlManager m;
    QVERIFY(!m.canStart());
    QVERIFY(m.avatarUrl().isEmpty());
}

#include "moc_downloadavartarurlmanagertest.cpp"
