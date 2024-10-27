/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelinewidgettest.h"
#include "room/messagelinewidget.h"
#include "room/messagetextedit.h"
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>

QTEST_MAIN(MessageLineWidgetTest)

MessageLineWidgetTest::MessageLineWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void MessageLineWidgetTest::shouldHaveDefaultValues()
{
    MessageLineWidget w;
    QCOMPARE(w.mode(), MessageLineWidget::EditingMode::NewMessage);
    QVERIFY(w.threadMessageId().isEmpty());

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    QCOMPARE(mainLayout->spacing(), 0);

    auto mMessageTextEdit = w.findChild<MessageTextEdit *>(QStringLiteral("mMessageTextEdit"));
    QVERIFY(mMessageTextEdit);

    auto mSendFileButton = w.findChild<QToolButton *>(QStringLiteral("mSendFileButton"));
    QVERIFY(mSendFileButton);
    QVERIFY(!mSendFileButton->icon().isNull());
    QVERIFY(mSendFileButton->autoRaise());
    QVERIFY(!mSendFileButton->toolTip().isEmpty());

    auto mEmoticonButton = w.findChild<QToolButton *>(QStringLiteral("mEmoticonButton"));
    QVERIFY(mEmoticonButton);
    QVERIFY(!mEmoticonButton->icon().isNull());
    QCOMPARE(mEmoticonButton->popupMode(), QToolButton::InstantPopup);
    QVERIFY(mEmoticonButton->autoRaise());
    QVERIFY(!mEmoticonButton->toolTip().isEmpty());

    auto mSendMessageButton = w.findChild<QToolButton *>(QStringLiteral("mSendMessageButton"));
    QVERIFY(mSendMessageButton);
    QVERIFY(!mSendMessageButton->icon().isNull());
    QVERIFY(mSendMessageButton->autoRaise());
    QVERIFY(!mSendMessageButton->toolTip().isEmpty());

    auto mVideoMessageButton = w.findChild<QToolButton *>(QStringLiteral("mVideoMessageButton"));
    QVERIFY(mVideoMessageButton);
    QVERIFY(!mVideoMessageButton->icon().isNull());
    QVERIFY(mVideoMessageButton->autoRaise());
    QVERIFY(!mVideoMessageButton->toolTip().isEmpty());

    auto mSoundMessageButton = w.findChild<QToolButton *>(QStringLiteral("mSoundMessageButton"));
    QVERIFY(mSoundMessageButton);
    QVERIFY(!mSoundMessageButton->icon().isNull());
    QVERIFY(mSoundMessageButton->autoRaise());
    QVERIFY(!mSoundMessageButton->toolTip().isEmpty());

    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.messageIdBeingEdited().isEmpty());

    QVERIFY(w.quoteText().isEmpty());
    QVERIFY(w.quotePermalink().isEmpty());
}

#include "moc_messagelinewidgettest.cpp"
