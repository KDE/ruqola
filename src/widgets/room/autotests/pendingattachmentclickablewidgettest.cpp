/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pendingattachmentclickablewidgettest.h"
#include "room/pendingattachmentclickablewidget.h"
#include <QLabel>
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

    auto mNameLabel = w.findChild<QLabel *>(u"nameLabel"_s);
    QVERIFY(mNameLabel);
    QVERIFY(mNameLabel->text().isEmpty());

    auto mSizeLabel = w.findChild<QLabel *>(u"sizeLabel"_s);
    QVERIFY(mSizeLabel);
    QVERIFY(mSizeLabel->text().isEmpty());

    auto mIconLabel = w.findChild<QLabel *>(u"iconLabel"_s);
    QVERIFY(mIconLabel);
    QVERIFY(mIconLabel->text().isEmpty());
}
#include "moc_pendingattachmentclickablewidgettest.cpp"
