/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "selectedmessagebackgroundanimationtest.h"

#include "room/selectedmessagebackgroundanimation.h"
#include <QSignalSpy>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(SelectedMessageBackgroundAnimationTest)
SelectedMessageBackgroundAnimationTest::SelectedMessageBackgroundAnimationTest(QObject *parent)
    : QObject{parent}
{
}

void SelectedMessageBackgroundAnimationTest::shouldHaveDefaultValues()
{
    const SelectedMessageBackgroundAnimation d(nullptr);
    QVERIFY(!d.backgroundColor().isValid());
    QVERIFY(!d.modelIndex().isValid());
}

void SelectedMessageBackgroundAnimationTest::shouldEmitBackgroundColorChanged()
{
    SelectedMessageBackgroundAnimation d(nullptr);
    QSignalSpy backgroundColorChangedspy(&d, &SelectedMessageBackgroundAnimation::backgroundColorChanged);
    const QColor color(Qt::red);
    d.setBackgroundColor(color);
    QCOMPARE(backgroundColorChangedspy.count(), 1);
    backgroundColorChangedspy.clear();
    d.setBackgroundColor(color);
    QCOMPARE(backgroundColorChangedspy.count(), 0);
    backgroundColorChangedspy.clear();

    const QColor colorBlack(Qt::black);
    d.setBackgroundColor(colorBlack);
    QCOMPARE(backgroundColorChangedspy.count(), 1);
}

#include "moc_selectedmessagebackgroundanimationtest.cpp"
