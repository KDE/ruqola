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
}

void MessageListDelegateTest::layoutChecks()
{
    // GIVEN a delegate and an index pointing to a message
    MessageListDelegate delegate;
    delegate.setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    option.rect = QRect(100, 100, 500, 500);
    Message message;
    message.setUserId(QStringLiteral("userId"));
    message.setUsername(QStringLiteral("dfaure"));
    message.setTimeStamp(QDateTime(QDate(2020, 2, 1), QTime(4, 7, 15)).toMSecsSinceEpoch());
    const MessageAttachment msgAttach = testAttachment();
    message.setAttachements({msgAttach});

    QStandardItemModel model;
    QStandardItem *item = new QStandardItem;
    item->setData(message.username(), MessageModel::Username);
    item->setData(message.userId(), MessageModel::UserId);
    item->setData(true, MessageModel::DateDiffersFromPrevious);
    item->setData(message.displayTime(), MessageModel::Timestamp);
    item->setData(QVariant::fromValue(&message), MessageModel::MessagePointer);
    model.setItem(0, 0, item);
    const QModelIndex index = model.index(0, 0);

    // WHEN
    const MessageListDelegate::Layout layout = delegate.doLayout(option, index);

    // THEN
    QCOMPARE(layout.senderText, QStringLiteral("@dfaure"));
    QCOMPARE(layout.timeStampText, QStringLiteral("04:07"));
    QVERIFY(layout.timeSize.isValid());
    QVERIFY(option.rect.contains(layout.usableRect));
    QVERIFY(option.rect.contains(layout.senderRect.toRect()));
    QVERIFY(option.rect.contains(layout.attachmentsRect));
    QVERIFY(!layout.textRect.isValid());

    // Add a text message, the height should increase
    message.setText(QStringLiteral("The <b>text</b>"));
    item->setData(message.text(), MessageModel::OriginalMessage);
    item->setData(message.text(), MessageModel::MessageConvertedText);

    // WHEN
    const MessageListDelegate::Layout layoutWithText = delegate.doLayout(option, index);

    // THEN
    QVERIFY(option.rect.contains(layoutWithText.textRect));
    QVERIFY(option.rect.contains(layout.senderRect.toRect()));
    QCOMPARE(layoutWithText.usableRect.left(), layoutWithText.textRect.left());
    QVERIFY(layoutWithText.textRect.top() >= layoutWithText.usableRect.top());
    QVERIFY(!layoutWithText.senderRect.intersects(layoutWithText.textRect));
    QVERIFY(layoutWithText.textRect.height() >= layout.textRect.height() + 4);
    QCOMPARE(layoutWithText.attachmentsRect.top(), layoutWithText.textRect.bottom());
    QVERIFY(option.rect.contains(layout.attachmentsRect));

}
