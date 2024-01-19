/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewurlcachemanagertest.h"
#include "previewurlcachemanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(PreviewUrlCacheManagerTest)

PreviewUrlCacheManagerTest::PreviewUrlCacheManagerTest(QObject *parent)
    : QObject{parent}
{
}

void PreviewUrlCacheManagerTest::shouldHaveDefaultValues()
{
    PreviewUrlCacheManager w;
    QCOMPARE(w.embedCacheExpirationDays(), -1);
    // TODO
}
