/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasewidgettest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchMessageLineWidget = w.findChild<SearchMessageLineWidget *>(QStringLiteral("mSearchMessageLineWidget"));
    QVERIFY(mSearchMessageLineWidget);

    auto mMessageListInfo = w.findChild<QLabel *>(QStringLiteral("mMessageListInfo"));
    QVERIFY(mMessageListInfo);
    QVERIFY(mMessageListInfo->text().isEmpty());
    QCOMPARE(mMessageListInfo->textFormat(), Qt::RichText);
    QCOMPARE(mMessageListInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);
    QCOMPARE(mMessageListView->mode(), MessageListView::Mode::Viewing);
}

#include "moc_showlistmessagebasewidgettest.cpp"
