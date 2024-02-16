/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidgettest.h"
#include "common/flowlayout.h"
#include "forwardmessage/forwardmessagewidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ForwardMessageWidgetTest)
ForwardMessageWidgetTest::ForwardMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageWidgetTest::shouldHaveDefaultValues()
{
    ForwardMessageWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mFlowLayout = w.findChild<FlowLayout *>(QStringLiteral("mFlowLayout"));
    QVERIFY(mFlowLayout);

    auto mForwardMessageAddChannelCompletionLineEdit =
        w.findChild<ForwardMessageAddChannelCompletionLineEdit *>(QStringLiteral("mForwardMessageAddChannelCompletionLineEdit"));
    QVERIFY(mForwardMessageAddChannelCompletionLineEdit);
}

#include "moc_forwardmessagewidgettest.cpp"
