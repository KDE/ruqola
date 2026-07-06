/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clickablewidgettest.h"

#include "misc/clickablewidget.h"
#include <QTest>
QTEST_MAIN(ClickableWidgetTest)
using namespace Qt::Literals::StringLiterals;
ClickableWidgetTest::ClickableWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ClickableWidgetTest::shouldHaveDefaultValues()
{
    const ClickableWidget w(u"foo"_s);
    QVERIFY(!w.name().isEmpty()); // See foo in constructor
    QVERIFY(w.identifier().isEmpty());

    auto mLabel = w.findChild<QLabel *>(u"mUserLabel"_s);
    QVERIFY(mLabel);

    auto mClickableLabel = w.findChild<ClickableLabel *>(u"mClickableLabel"_s);
    QVERIFY(mClickableLabel);

    auto mUserPixmap = w.findChild<QLabel *>(u"mUserPixmap"_s);
    QVERIFY(mUserPixmap);
}

#include "moc_clickablewidgettest.cpp"
