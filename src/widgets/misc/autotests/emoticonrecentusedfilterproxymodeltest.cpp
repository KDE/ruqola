/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emoticonrecentusedfilterproxymodeltest.h"
#include "misc/emoticonrecentusedfilterproxymodel.h"
#include <QTest>
QTEST_MAIN(EmoticonRecentUsedFilterProxyModelTest)
EmoticonRecentUsedFilterProxyModelTest::EmoticonRecentUsedFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonRecentUsedFilterProxyModelTest::shouldHaveDefaultValues()
{
    EmoticonRecentUsedFilterProxyModel w;
    QVERIFY(w.usedIdentifier().isEmpty());
}
