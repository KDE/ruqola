/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roomwidgetbasetest.h"
#include "room/messagelinewidget.h"
#include "room/readonlylineeditwidget.h"
#include "room/roomquotemessagewidget.h"
#include "room/roomreplythreadwidget.h"
#include "room/roomwidgetbase.h"
#include "room/uploadfileprogressstatuswidget.h"
#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RoomWidgetBaseTest)

RoomWidgetBaseTest::RoomWidgetBaseTest(QObject *parent)
    : QObject(parent)
{
}

void RoomWidgetBaseTest::shouldHaveDefaultValues()
{
    RoomWidgetBase w(MessageListView::Mode::Editing);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mUploadFileProgressStatusWidget = w.findChild<UploadFileProgressStatusWidget *>(QStringLiteral("mUploadFileProgressStatusWidget"));
    QVERIFY(mUploadFileProgressStatusWidget);

    auto mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    QCOMPARE(mStackedWidget->count(), 2);

    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(QStringLiteral("mMessageLineWidget"));
    QVERIFY(mMessageLineWidget);

    auto mReadOnlyLineEditWidget = w.findChild<ReadOnlyLineEditWidget *>(QStringLiteral("mReadOnlyLineEditWidget"));
    QVERIFY(mReadOnlyLineEditWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mMessageLineWidget);

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(QStringLiteral("mRoomReplyThreadWidget"));
    QVERIFY(mRoomReplyThreadWidget);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mRoomQuoteMessageWidget = w.findChild<RoomQuoteMessageWidget *>(QStringLiteral("mRoomQuoteMessageWidget"));
    QVERIFY(mRoomQuoteMessageWidget);
    QVERIFY(!mRoomQuoteMessageWidget->isVisible());
}
