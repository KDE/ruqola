/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagelistdelegatetest.h"
#include "room/messagelistdelegate.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messagedelegatehelperimagetest.h"
#include "messages/message.h"
#include "messages/messageattachment.h"
#include "testdata.h"

#include <QStandardItemModel>
#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageListDelegateTest)

MessageListDelegateTest::MessageListDelegateTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
    Ruqola::self()->rocketChatAccount()->setAccountName(QStringLiteral("accountName"));
}

void MessageListDelegateTest::layoutChecks_data()
{
    QTest::addColumn<Message>("message");
    QTest::addColumn<bool>("withDateHeader");

    Message message;
    message.setUserId(QStringLiteral("userId"));
    message.setUsername(QStringLiteral("dfaure"));
    message.setTimeStamp(QDateTime(QDate(2020, 2, 1), QTime(4, 7, 15)).toMSecsSinceEpoch());
    message.setMessageType(Message::NormalText);

    QTest::newRow("text_no_date") << message << false;
    QTest::newRow("text_with_date") << message << true;

    message.setMessageType(Message::Image);
    const MessageAttachment msgAttach = testAttachment();
    message.setAttachements({msgAttach});

    QTest::newRow("attachment_no_text_no_date") << message << false;
    QTest::newRow("attachment_no_text_with_date") << message << true;

    // TODO tests with reactions

    message.setText(QStringLiteral("The <b>text</b>"));

    QTest::newRow("attachment_with_text_no_date") << message << false;
    QTest::newRow("attachment_with_text_with_date") << message << true;

}

void MessageListDelegateTest::layoutChecks()
{
    QFETCH(Message, message);
    QFETCH(bool, withDateHeader);

    // GIVEN a delegate and an index pointing to a message
    MessageListDelegate delegate;
    delegate.setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    option.rect = QRect(100, 100, 500, 500);

    QStandardItemModel model;
    QStandardItem *item = new QStandardItem;
    item->setData(message.username(), MessageModel::Username);
    item->setData(message.userId(), MessageModel::UserId);
    item->setData(withDateHeader, MessageModel::DateDiffersFromPrevious);
    item->setData(message.displayTime(), MessageModel::Timestamp);
    item->setData(QVariant::fromValue(&message), MessageModel::MessagePointer);
    item->setData(message.text(), MessageModel::OriginalMessage);
    item->setData(message.text(), MessageModel::MessageConvertedText);
    model.setItem(0, 0, item);
    const QModelIndex index = model.index(0, 0);

    // WHEN calculating sizehint
    const QSize sizeHint = delegate.sizeHint(option, index);
    QVERIFY(sizeHint.isValid());
    option.rect.setSize(sizeHint);

    // ... and redoing layout while painting
    const MessageListDelegate::Layout layout = delegate.doLayout(option, index);

    // THEN
    QCOMPARE(layout.senderText, QStringLiteral("@dfaure"));
    QCOMPARE(layout.timeStampText, QStringLiteral("04:07"));
    QVERIFY(layout.timeSize.isValid());
    QVERIFY(option.rect.contains(layout.usableRect));
    QVERIFY(option.rect.contains(layout.senderRect.toRect()));
    if (message.attachements().isEmpty()) {
        QVERIFY(layout.attachmentsRect.isNull());
    } else {
        QVERIFY(sizeHint.height() > layout.senderRect.height() + 1);
        QVERIFY(option.rect.contains(layout.attachmentsRect));
    }
    if (message.text().isEmpty()) {
        QVERIFY(!layout.textRect.isValid());
    } else {
        QVERIFY(option.rect.contains(layout.textRect));
        QCOMPARE(layout.usableRect.left(), layout.textRect.left());
        QVERIFY(layout.textRect.top() >= layout.usableRect.top());
        QVERIFY(!layout.senderRect.intersects(layout.textRect));
        if (!message.attachements().isEmpty()) {
            QCOMPARE(layout.attachmentsRect.top(), layout.textRect.bottom());
        }
    }
}
