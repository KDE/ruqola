/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatetest.h"
using namespace Qt::Literals::StringLiterals;

#include "messages/message.h"
#include "messages/messageattachment.h"
#include "rocketchataccount.h"
#include "room/delegate/messagelistdelegate.h"
#include "ruqola.h"
#include "testdata.h"

#include <QStandardItemModel>
#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageListDelegateTest)

MessageListDelegateTest::MessageListDelegateTest(QObject *parent)
    : QObject(parent)
{
    initTestAccount();

    const QString userId = u"dfaureUserId"_s;
    Ruqola::self()->rocketChatAccount()->insertAvatarUrl(userId, avatarLink());
}

void MessageListDelegateTest::layoutChecks_data()
{
    QTest::addColumn<Message>("message");
    QTest::addColumn<bool>("withDateHeader");

    Message message;
    message.setMessageId("someNonEmptyId"_ba);
    message.setUserId("dfaureUserId"_ba);
    message.setUsername(u"dfaure"_s);
    message.setTimeStamp(QDateTime(QDate(2020, 2, 1), QTime(4, 7, 15)).toMSecsSinceEpoch());
    message.setMessageType(Message::NormalText);

    QTest::newRow("text_no_date") << message << false;
    QTest::newRow("text_with_date") << message << true;

    message.setMessageType(Message::NormalText);
    MessageAttachment msgAttach = testAttachment();
    msgAttach.setAttachmentType(MessageAttachment::AttachmentType::Image);

    MessageAttachments attachments;
    QList<MessageAttachment> attachmentInfos;
    attachmentInfos.append(msgAttach);
    attachments.setMessageAttachments(attachmentInfos);

    message.setAttachments(attachments);

    QTest::newRow("attachment_no_text_no_date") << message << false;
    QTest::newRow("attachment_no_text_with_date") << message << true;

    message.setText(u"The <b>text</b>"_s);

    QTest::newRow("attachment_with_text_no_date") << message << false;
    QTest::newRow("attachment_with_text_with_date") << message << true;

    message.setEditedByUsername(message.username());

    QTest::newRow("edited_with_attachment_with_text_with_date") << message << true;

    // TODO tests with reactions
}

void MessageListDelegateTest::layoutChecks()
{
    QFETCH(Message, message);
    QFETCH(bool, withDateHeader);

    // GIVEN a delegate and an index pointing to a message
    MessageListDelegate delegate(Ruqola::self()->rocketChatAccount(), nullptr);
    delegate.setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    option.rect = QRect(100, 100, 500, 500);

    QStandardItemModel model;
    auto item = new QStandardItem;
    item->setData(message.username(), MessagesModel::Username);
    item->setData(message.userId(), MessagesModel::UserId);
    item->setData(withDateHeader, MessagesModel::DateDiffersFromPrevious);
    item->setData(message.displayTime(), MessagesModel::Timestamp);
    item->setData(QVariant::fromValue(&message), MessagesModel::MessagePointer);
    item->setData(message.text(), MessagesModel::OriginalMessage);
    item->setData(message.text(), MessagesModel::MessageConvertedText);
    model.setItem(0, 0, item);
    // Ensure it's not last, that's a special case in sizeHint
    auto item2 = new QStandardItem;
    model.setItem(1, 0, item2);

    const QModelIndex index = model.index(0, 0);

    // WHEN calculating sizehint
    const QSize sizeHint = delegate.sizeHint(option, index);
    QVERIFY(sizeHint.isValid());
    option.rect.setSize(sizeHint);

    // ... and redoing layout while painting
    const MessageListLayoutBase::Layout layout = delegate.doLayout(option, index);

    // THEN
    QCOMPARE(layout.senderText, u"@dfaure"_s);
    QCOMPARE(layout.timeStampText, u"04:07"_s);
    QVERIFY(option.rect.contains(layout.usableRect));
    // FIXME: reactivate
    // QVERIFY(option.rect.contains(layout.senderRect.toRect()));
    if (!message.attachments()) {
        QVERIFY(layout.attachmentsRect.isNull());
    } else {
        QVERIFY(sizeHint.height() > layout.senderRect.height() + 1);
        QVERIFY(option.rect.contains(layout.attachmentsRect));
    }

    // Text
    if (message.text().isEmpty()) {
        QVERIFY(!layout.textRect.isValid());
    } else {
        QVERIFY(option.rect.contains(layout.textRect));
        QCOMPARE(layout.usableRect.left(), layout.textRect.left());
        QVERIFY(layout.textRect.top() >= layout.usableRect.top());
        QVERIFY(!layout.senderRect.intersects(layout.textRect));
        if (message.attachments() && !message.attachments()->isEmpty()) {
            QCOMPARE(layout.attachmentsRect.top(), layout.textRect.y() + layout.textRect.height());
        }
    }

#if 0 // Need to reactivate it
    const int bottom = layout.usableRect.y() + layout.usableRect.height();
    // Avatar
    QCOMPARE(qRound(layout.avatarPixmap.height() / layout.avatarPixmap.devicePixelRatioF()), layout.senderRect.height());
    QCOMPARE(layout.avatarPixmap.devicePixelRatioF(), fakeWidget.devicePixelRatioF());
    // qDebug() << layout.avatarPos.y() << "+" << layout.avatarPixmap.height() << "must be <=" << bottom;
    QVERIFY(layout.avatarPos.y() + layout.avatarPixmap.height() / layout.avatarPixmap.devicePixelRatioF() <= bottom);
    // Reactions
    auto react = message.reactions();
    if (react && react->isEmpty()) {
        QCOMPARE(layout.reactionsHeight, 0);
    } else {
        QVERIFY(layout.reactionsHeight > 15);
        QVERIFY(layout.reactionsY + layout.reactionsHeight <= bottom);
    }
#endif

    // Edited
    if (message.wasEdited()) {
        QVERIFY(option.rect.contains(layout.editedIconRect));
        QVERIFY(!layout.editedIconRect.intersects(layout.textRect));
        QVERIFY(!layout.editedIconRect.intersects(layout.senderRect.toRect()));
    }
}

#include "moc_messagelistdelegatetest.cpp"
