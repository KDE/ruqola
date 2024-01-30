/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "switchchanneltreeviewmanagertest.h"
#include "switchchannelhistory/switchchanneltreeviewmanager.h"
#include <QTest>
QTEST_MAIN(SwitchChannelTreeViewManagerTest)

SwitchChannelTreeViewManagerTest::SwitchChannelTreeViewManagerTest(QObject *parent)
    : QObject{parent}
{
}

void SwitchChannelTreeViewManagerTest::shouldHaveDefaultValues()
{
    SwitchChannelTreeViewManager m;
    QVERIFY(!m.parentWidget());
    QVERIFY(m.switchChannelTreeView());
    QVERIFY(!m.currentRocketChatAccount());
}

#include "moc_switchchanneltreeviewmanagertest.cpp"
