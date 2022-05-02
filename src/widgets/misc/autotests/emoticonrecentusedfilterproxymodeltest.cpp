/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonrecentusedfilterproxymodeltest.h"
#include "misc/emoticonrecentusedfilterproxymodel.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(EmoticonRecentUsedFilterProxyModelTest)
EmoticonRecentUsedFilterProxyModelTest::EmoticonRecentUsedFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void EmoticonRecentUsedFilterProxyModelTest::shouldHaveDefaultValues()
{
    EmoticonRecentUsedFilterProxyModel w;
    QVERIFY(w.usedIdentifier().isEmpty());
}
