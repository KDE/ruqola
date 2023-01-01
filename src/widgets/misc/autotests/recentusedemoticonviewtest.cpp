/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "recentusedemoticonviewtest.h"
#include "misc/recentusedemoticonview.h"
#include <QTest>
QTEST_MAIN(RecentUsedEmoticonViewTest)
RecentUsedEmoticonViewTest::RecentUsedEmoticonViewTest(QObject *parent)
    : QObject(parent)
{
}

void RecentUsedEmoticonViewTest::shouldHaveDefaultValues()
{
    RecentUsedEmoticonView w;
    // TODO
}
