/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimationtest.h"
#include "room/selectedmessagebackgroundanimation.h"
#include <QTest>
QTEST_MAIN(SelectedMessageBackgroundAnimationTest)
SelectedMessageBackgroundAnimationTest::SelectedMessageBackgroundAnimationTest(QObject *parent)
    : QObject{parent}
{
}

void SelectedMessageBackgroundAnimationTest::shouldHaveDefaultValues()
{
    SelectedMessageBackgroundAnimation d(nullptr);
    QVERIFY(!d.backgroundColor().isValid());
    QVERIFY(!d.messageModel());
    QVERIFY(!d.modelIndex().isValid());
}

#include "moc_selectedmessagebackgroundanimationtest.cpp"
