/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasewidgettest.h"
#include "dialogs/showlistmessagebasewidget.h"
#include "room/messagelistview.h"
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ShowListMessageBaseWidgetTest)
ShowListMessageBaseWidgetTest::ShowListMessageBaseWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowListMessageBaseWidgetTest::shouldHaveDefaultValues()
{
    ShowListMessageBaseWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mSearchMessageLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchMessageLineEdit"));
    QVERIFY(mSearchMessageLineEdit);
    QVERIFY(mSearchMessageLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchMessageLineEdit->placeholderText().isEmpty());

    auto mMessageListInfo = w.findChild<QLabel *>(QStringLiteral("mMessageListInfo"));
    QVERIFY(mMessageListInfo);
    QVERIFY(mMessageListInfo->text().isEmpty());
    QCOMPARE(mMessageListInfo->textFormat(), Qt::RichText);
    QCOMPARE(mMessageListInfo->contextMenuPolicy(), Qt::NoContextMenu);

    auto mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);
    QCOMPARE(mMessageListView->mode(), MessageListView::Mode::Viewing);
}
