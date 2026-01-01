/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mFlowLayout = w.findChild<FlowLayout *>(u"mFlowLayout"_s);
    QVERIFY(mFlowLayout);

    auto mForwardMessageAddChannelCompletionLineEdit =
        w.findChild<ForwardMessageAddChannelCompletionLineEdit *>(u"mForwardMessageAddChannelCompletionLineEdit"_s);
    QVERIFY(mForwardMessageAddChannelCompletionLineEdit);
}

#include "moc_forwardmessagewidgettest.cpp"
