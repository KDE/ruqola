/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateversionwidgettest.h"
#include "needupdateversion/needupdateversionwidget.h"
#include <QTest>
QTEST_MAIN(NeedUpdateVersionWidgetTest)
NeedUpdateVersionWidgetTest::NeedUpdateVersionWidgetTest(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateVersionWidgetTest::~NeedUpdateVersionWidgetTest() = default;

void NeedUpdateVersionWidgetTest::shouldHaveDefaultValues()
{
    NeedUpdateVersionWidget w;
    QCOMPARE(w.position(), KMessageWidget::Header);
}

#include "moc_needupdateversionwidgettest.cpp"
