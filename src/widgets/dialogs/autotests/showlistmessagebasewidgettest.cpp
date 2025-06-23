/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/searchmessagelinewidget.h"
#include "dialogs/showlistmessagebasewidget.h"
#include "room/messagelistview.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ShowListMessageBaseWidgetTest)
ShowListMessageBaseWidgetTest::ShowListMessageBaseWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowListMessageBaseWidgetTest::shouldHaveDefaultValues()
{
    ShowListMessageBaseWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchMessageLineWidget = w.findChild<SearchMessageLineWidget *>(u"mSearchMessageLineWidget"_s);
    QVERIFY(mSearchMessageLineWidget);

    auto mMessageListInfo = w.findChild<QLabel *>(u"mMessageListInfo"_s);
    QVERIFY(mMessageListInfo);
    QVERIFY(mMessageListInfo->text().isEmpty());
    QCOMPARE(mMessageListInfo->textFormat(), Qt::RichText);
    QCOMPARE(mMessageListInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mMessageListView = w.findChild<MessageListView *>(u"mMessageListView"_s);
    QVERIFY(mMessageListView);
    QCOMPARE(mMessageListView->mode(), MessageListView::Mode::Viewing);
}

#include "moc_showlistmessagebasewidgettest.cpp"
