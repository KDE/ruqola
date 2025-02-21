/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanagertest.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(ActionButtonsManagerTest)
ActionButtonsManagerTest::ActionButtonsManagerTest(QObject *parent)
    : QObject{parent}
{
}

void ActionButtonsManagerTest::shouldHaveDefaultValues()
{
    ActionButtonsManager m;
    QVERIFY(m.actionButtons().isEmpty());
}

#include "moc_actionbuttonsmanagertest.cpp"
