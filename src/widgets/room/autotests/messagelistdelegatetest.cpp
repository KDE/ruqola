/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatetest.h"

#include "messages/message.h"
#include "messages/messageattachment.h"
#include "rocketchataccount.h"
#include "room/delegate/messagelistdelegate.h"
#include "ruqola.h"
#include "ruqolaglobalconfig.h"
#include "testdata.h"

#include <QStandardItemModel>
#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageListDelegateTest)

using namespace Qt::Literals::StringLiterals;
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
    QTest::addColumn<bool>("normalLayout");

    Message message;
    message.setMessageId("someNonEmptyId"_ba);
    message.setUserId("dfaureUserId"_ba);
    message.setUsername(u"dfaure"_s);
    message.setTimeStamp(QDateTime(QDate(2020, 2, 1), QTime(4, 7, 15)).toMSecsSinceEpoch());
    message.setMessageType(Message::NormalText);

    QTest::newRow("text_no_date") << message << false << false;
    QTest::newRow("text_with_date") << message << true << false;

    Message messageWithLargeEmoji = message;
    messageWithLargeEmoji.setText(uR"(<span style="font: x-large NotoColorEmoji">💰</span> Text)"_s);
    QTest::newRow("large_emoji_no_date") << messageWithLargeEmoji << false << true;

    message.setMessageType(Message::NormalText);
    MessageAttachment msgAttach = testAttachment();
    msgAttach.setAttachmentType(MessageAttachment::AttachmentType::Image);

    MessageAttachments attachments;
    QList<MessageAttachment> attachmentInfos;
    attachmentInfos.append(msgAttach);
    attachments.setMessageAttachments(attachmentInfos);

    message.setAttachments(attachments);

    QTest::newRow("attachment_no_text_no_date") << message << false << false;
    QTest::newRow("attachment_no_text_with_date") << message << true << false;

    message.setText(u"The <b>text</b>"_s);

    QTest::newRow("attachment_with_text_no_date") << message << false << false;
    QTest::newRow("attachment_with_text_with_date") << message << true << false;

    message.setEditedByUsername(message.username());

    QTest::newRow("edited_with_attachment_with_text_with_date") << message << true << false;

    // TODO tests with reactions
}

void MessageListDelegateTest::layoutChecks()
{
    QFETCH(Message, message);
    QFETCH(bool, withDateHeader);
    QFETCH(bool, normalLayout);

    // GIVEN a delegate and an index pointing to a message
    const auto previousMessageStyle = RuqolaGlobalConfig::self()->messageStyle();
    if (normalLayout) {
        RuqolaGlobalConfig::self()->setMessageStyle(RuqolaGlobalConfig::EnumMessageStyle::Normal);
    }
    MessageListDelegate delegate(Ruqola::self()->rocketChatAccount(), nullptr);
    delegate.setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    RuqolaGlobalConfig::self()->setMessageStyle(previousMessageStyle);
    QStyleOptionViewItem option;
    const QWidget fakeWidget;
    option.widget = &fakeWidget;
    option.rect = QRect(normalLayout ? 0 : 100, 100, 500, 500);

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
    QCOMPARE(layout.senderText, u"dfaure"_s);
    QCOMPARE(layout.timeStampText, normalLayout ? u"·  04:07"_s : u"04:07"_s);
    QVERIFY(option.rect.contains(layout.usableRect));
    if (normalLayout) {
        QCOMPARE(qRound(layout.senderRect.top()), layout.usableRect.top());
    }

    // Text
    if (message.text().isEmpty()) {
        QVERIFY(!layout.textRect.isValid());
    } else {
        QVERIFY(option.rect.contains(layout.textRect));
        QCOMPARE(layout.usableRect.left(), layout.textRect.left());
        QVERIFY(layout.textRect.top() >= layout.usableRect.top());
        QVERIFY(!layout.senderRect.intersects(layout.textRect));
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
