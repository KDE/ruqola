/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pendingattachmentwidgettest.h"
#include "room/pendingattachmentwidget.h"

#include <QTest>
QTEST_MAIN(PendingAttachmentWidgetTest)

PendingAttachmentWidgetTest::PendingAttachmentWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PendingAttachmentWidgetTest::shouldHaveDefaultValues()
{
    const PendingAttachmentWidget w;
    QVERIFY(!w.hasAttachments());
    QVERIFY(w.attachmentsInfo().isEmpty());
}

#include "moc_pendingattachmentwidgettest.cpp"
