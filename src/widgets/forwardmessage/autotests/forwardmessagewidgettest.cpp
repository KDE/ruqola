/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "forwardmessage/forwardmessagewidget.h"
#include "misc/clickablewidget.h"
#include <QLabel>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>

QTEST_MAIN(ForwardMessageWidgetTest)
ForwardMessageWidgetTest::ForwardMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageWidgetTest::shouldHaveDefaultValues()
{
    const ForwardMessageWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mFlowLayout = w.findChild<TextAddonsWidgets::TextAddonsWidgetFlowLayout *>(u"mFlowLayout"_s);
    QVERIFY(mFlowLayout);

    auto mForwardMessageAddChannelCompletionLineEdit =
        w.findChild<ForwardMessageAddChannelCompletionLineEdit *>(u"mForwardMessageAddChannelCompletionLineEdit"_s);
    QVERIFY(mForwardMessageAddChannelCompletionLineEdit);
}

void ForwardMessageWidgetTest::shouldAddChannelsWithIdenticalNames()
{
    ForwardMessageWidget w(nullptr);
    auto lineEdit = w.findChild<ForwardMessageAddChannelCompletionLineEdit *>(u"mForwardMessageAddChannelCompletionLineEdit"_s);
    QVERIFY(lineEdit);
    QSignalSpy updateOkButtonSpy(&w, &ForwardMessageWidget::updateOkButton);
    QVERIFY(w.channelIdentifiers().isEmpty());

    // Two distinct rooms can share the same display name (two direct messages for example)
    const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo firstChannel{
        .name = u"same-name"_s,
        .channelId = "channelid1",
    };
    const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo secondChannel{
        .name = u"same-name"_s,
        .channelId = "channelid2",
    };
    Q_EMIT lineEdit->forwardToChannel(firstChannel);
    Q_EMIT lineEdit->forwardToChannel(secondChannel);

    QCOMPARE(w.channelIdentifiers(), (QList<QByteArray>{"channelid1", "channelid2"}));
    QCOMPARE(updateOkButtonSpy.count(), 2);
    QVERIFY(updateOkButtonSpy.constLast().at(0).toBool());

    // Adding an already selected room doesn't add it twice
    Q_EMIT lineEdit->forwardToChannel(secondChannel);
    QCOMPARE(w.channelIdentifiers().count(), 2);
    QCOMPARE(updateOkButtonSpy.count(), 2);
}

void ForwardMessageWidgetTest::shouldRemoveChannel()
{
    ForwardMessageWidget w(nullptr);
    auto lineEdit = w.findChild<ForwardMessageAddChannelCompletionLineEdit *>(u"mForwardMessageAddChannelCompletionLineEdit"_s);
    QVERIFY(lineEdit);
    const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo firstChannel{
        .name = u"same-name"_s,
        .channelId = "channelid1",
    };
    const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo secondChannel{
        .name = u"same-name"_s,
        .channelId = "channelid2",
    };
    Q_EMIT lineEdit->forwardToChannel(firstChannel);
    Q_EMIT lineEdit->forwardToChannel(secondChannel);
    QCOMPARE(w.channelIdentifiers().count(), 2);

    QSignalSpy updateOkButtonSpy(&w, &ForwardMessageWidget::updateOkButton);
    const QList<ClickableWidget *> clickableWidgets = w.findChildren<ClickableWidget *>();
    QCOMPARE(clickableWidgets.count(), 2);
    for (ClickableWidget *clickableWidget : clickableWidgets) {
        if (clickableWidget->identifier() == "channelid1") {
            auto clickableLabel = clickableWidget->findChild<ClickableLabel *>(u"mClickableLabel"_s);
            QVERIFY(clickableLabel);
            QTest::mouseClick(clickableLabel, Qt::LeftButton);
        }
    }

    // Only the clicked room is removed, the one sharing its name is kept
    QCOMPARE(w.channelIdentifiers(), (QList<QByteArray>{"channelid2"}));
    QCOMPARE(updateOkButtonSpy.count(), 1);
    QVERIFY(updateOkButtonSpy.constLast().at(0).toBool());
}

#include "moc_forwardmessagewidgettest.cpp"
