/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
