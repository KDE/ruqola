/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "newmessageindicatortest.h"
#include "room/newmessageindicator.h"
#include <QSignalSpy>
#include <QTest>
#include <qtestmouse.h>
QTEST_MAIN(NewMessageIndicatorTest)

NewMessageIndicatorTest::NewMessageIndicatorTest(QObject *parent)
    : QObject{parent}
{
}

void NewMessageIndicatorTest::shouldHaveDefaultValues()
{
    const NewMessageIndicatorWidget w;
    QVERIFY(w.hasMouseTracking());
}

void NewMessageIndicatorTest::shouldNewMessageIndicatorWidgetEmitSignal()
{
    NewMessageIndicatorWidget w;

    const QSignalSpy newMessageIndicatorWidgetSignal(&w, &NewMessageIndicatorWidget::clicked);
    QTest::mouseClick(&w, Qt::LeftButton);
    QCOMPARE(newMessageIndicatorWidgetSignal.count(), 1);
}

#include "moc_newmessageindicatortest.cpp"
