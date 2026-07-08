/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pendingattachmentclickablewidgettest.h"
#include "room/pendingattachmentclickablewidget.h"
#include <QTest>
#include <QToolButton>
QTEST_MAIN(PendingAttachmentClickableWidgetTest)
using namespace Qt::Literals::StringLiterals;
PendingAttachmentClickableWidgetTest::PendingAttachmentClickableWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PendingAttachmentClickableWidgetTest::shouldHaveDefaultValues()
{
    const PendingAttachmentClickableWidget w({});
    auto removeBtn = w.findChild<QToolButton *>(u"removeBtn"_s);
    QVERIFY(removeBtn);
    QVERIFY(removeBtn->autoRaise());
    QVERIFY(!removeBtn->toolTip().isEmpty());
    // TODO
}
#include "moc_pendingattachmentclickablewidgettest.cpp"
