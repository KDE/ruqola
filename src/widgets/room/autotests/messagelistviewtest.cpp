/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewtest.h"
#include "room/messagelistview.h"
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTest>
QTEST_MAIN(MessageListViewTest)

MessageListViewTest::MessageListViewTest(QObject *parent)
    : QObject{parent}
{
}

void MessageListViewTest::shouldHaveDefaultValues()
{
    const MessageListView w(nullptr, MessageListView::Mode::Editing);
    QCOMPARE(w.mode(), MessageListView::Mode::Editing);
    QCOMPARE(w.selectionMode(), QAbstractItemView::NoSelection);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(w.verticalScrollMode(), QAbstractItemView::ScrollPerPixel);
    QVERIFY(w.wordWrap());
    QCOMPARE(w.focusPolicy(), Qt::NoFocus);
}

// Re-selecting the very same room (RuqolaMainWidget::selectChannelRoom() does it "During reconnect")
// hands setModel() the model it already has. It must not tear down the view<->model wiring.
void MessageListViewTest::shouldKeepViewLiveWhenReassigningSameModel()
{
    MessageListView w(nullptr, MessageListView::Mode::Viewing);
    // The real delegate requires a MessagesModel; this test only exercises the view<->model wiring.
    w.setItemDelegate(new QStyledItemDelegate(&w));
    QStandardItemModel model;
    model.appendRow(new QStandardItem(QStringLiteral("first")));
    w.setModel(&model);
    w.resize(400, 300);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    w.setModel(&model);

    model.appendRow(new QStandardItem(QStringLiteral("second")));
    const QModelIndex newIndex = model.index(1, 0);
    QVERIFY(newIndex.isValid());
    QVERIFY(!w.visualRect(newIndex).isEmpty());
}

#include "moc_messagelistviewtest.cpp"
