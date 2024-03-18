/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimationtest.h"
#include "room/selectedmessagebackgroundanimation.h"
#include <QSignalSpy>
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

void SelectedMessageBackgroundAnimationTest::shouldEmitBackgroundColorChanged()
{
    SelectedMessageBackgroundAnimation d(nullptr);
    QSignalSpy backgroundColorChangedspy(&d, &SelectedMessageBackgroundAnimation::backgroundColorChanged);
    QColor color(Qt::red);
    d.setBackgroundColor(color);
    QCOMPARE(backgroundColorChangedspy.count(), 1);
    backgroundColorChangedspy.clear();
    d.setBackgroundColor(color);
    QCOMPARE(backgroundColorChangedspy.count(), 0);
    backgroundColorChangedspy.clear();

    QColor colorBlack(Qt::black);
    d.setBackgroundColor(colorBlack);
    QCOMPARE(backgroundColorChangedspy.count(), 1);
}

#include "moc_selectedmessagebackgroundanimationtest.cpp"
