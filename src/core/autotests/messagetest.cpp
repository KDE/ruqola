/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetest.h"
#include "messages/message.h"
#include "ruqola_autotest_helper.h"
#include <QCborValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(MessageTest)
MessageTest::MessageTest(QObject *parent)
    : QObject(parent)
{
}

void MessageTest::shouldHaveDefaultValues()
{
    Message m;
    QVERIFY(!m.pendingMessage());
    QVERIFY(!m.showIgnoredMessage());
    QVERIFY(m.showTranslatedMessage());
    QVERIFY(!m.unread());
    QVERIFY(!m.isEditingMode());
    QVERIFY(!m.hoverHighlight());
    QVERIFY(m.localTranslation().isEmpty());
    QVERIFY(!m.blocks());
    QVERIFY(!m.attachments());
    QCOMPARE(m.discussionCount(), 0);
    QVERIFY(!m.privateMessage());
    QVERIFY(!m.textToSpeechInProgress());
    // 14/03/2024 => size 816
    QCOMPARE(sizeof(Message), 432);
    QCOMPARE(m.messageStates(), Message::MessageStates(Message::MessageState::Groupable | Message::MessageState::Translated));
}

// TODO add check for default value ???

void MessageTest::shouldParseMessage_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Message>("expectedMessage");

    {
        Message firstMessageRef;
        firstMessageRef.setMessageId("blaid"_ba);
        firstMessageRef.setAlias(u"aliasname"_s);
        firstMessageRef.setGroupable(false);
        firstMessageRef.setTimeStamp(1504509615057);
        firstMessageRef.setUsername(u"foo.ff"_s);
        firstMessageRef.setName(u"newname"_s);
        firstMessageRef.setUserId("qt9uNY9FxyL5QT5no"_ba);
        firstMessageRef.setMessageType(Message::NormalText);
        firstMessageRef.setUpdatedAt(1504509615063);
        firstMessageRef.setEditedAt(-1);
        firstMessageRef.setIsStarred(false);
        firstMessageRef.setRoomId("qt9uNY9FxyL5QT5nouKK39zoewTkdacidH"_ba);
        MessageAttachment att;
        att.setLink(u"/file-upload/3zfLR3Cjr8YnvD6cS/dd.pdf"_s);
        att.setTitle(u"dd.pdf"_s);
        att.setAttachmentType(MessageAttachment::AttachmentType::File);
        att.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(att);
        attachments.setMessageAttachments(attachmentInfos);

        firstMessageRef.setAttachments(attachments);
        QTest::addRow("first") << u"first"_s << firstMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId("9kHnbbjbHKHjXXQp7"_ba);
        urlMessageRef.setAlias(u"alias"_s);
        urlMessageRef.setGroupable(false);
        urlMessageRef.setName(u"name"_s);
        urlMessageRef.setTimeStamp(1504596899771);
        urlMessageRef.setUsername(u"username"_s);
        urlMessageRef.setUserId("bjBueJtHsHQECdkmH"_ba);
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1504596901803);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setIsStarred(false);
        urlMessageRef.setRoomId("dBWXYy4nyBHn8Q7dv"_ba);
        urlMessageRef.setText(u"https://foo.com/event/whoa"_s);
        MessageUrl url;
        url.setUrl(u"https://foo.com/event/C8CT"_s);
        url.setPageTitle(u"Trainers Panel I"_s);
        url.setDescription(u"View more about this event at CppCon 2017"_s);
        url.setImageUrl(u"http://foo.com/img/app-icon.png?1504596901"_s);
        url.setSiteUrl(u"https://foo.com/event/C8CT"_s);
        url.setUrlId("9kHnbbjbHKHjXXQp7_0"_ba);
        url.setImageWidth(200);
        url.setImageHeight(200);
        url.generateMessageUrlInfo();
        MessageUrls u;
        u.setMessageUrls({url});
        urlMessageRef.setUrls(u);

        QTest::addRow("url") << u"url"_s << urlMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId("yZPPxBQ79M9jG5hS6"_ba);
        urlMessageRef.setGroupable(false);
        urlMessageRef.setName(u"test1 test2"_s);
        urlMessageRef.setTimeStamp(1704484544725);
        urlMessageRef.setUsername(u"test1"_s);
        urlMessageRef.setUserId("uKK39zoekdacidH"_ba);
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1704484544886);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setIsStarred(false);
        urlMessageRef.setParseUrls(true);
        urlMessageRef.setRoomId("wWTrQstz3mex5Mm"_ba);
        urlMessageRef.setText(u"https://foo/pull/48/files#diff-142b45cf8aa373eab25fd7a81c45c11fb2bb07e4556b34162497ecb66b4aefb4R149"_s);
        MessageUrl url;
        url.setUrl(u"https://foo/pull/48/files#diff-142b45cf8aa373eab25fd7a81c45c11fb2bb07e4556b34162497ecb66b4aefb4R149"_s);
        url.setPageTitle(u"Deferred connection evaluation by blabla · Pull Request #48 · "_s);
        url.setDescription(u"Closes #23"_s);
        url.setImageUrl(u"https://avatar.foo.com/u/84974957?s=400&v=4"_s);
        url.setSiteUrl(u"https://foo/pull/48"_s);
        url.setSiteName(u"GitHub"_s);
        url.setImageWidth(1200);
        url.setImageHeight(600);
        url.setUrlId("yZPPxBQ79M9jG5hS6_0"_ba);
        url.generateMessageUrlInfo();
        MessageUrls u;
        u.setMessageUrls({url});
        urlMessageRef.setUrls(u);
        QTest::addRow("url1") << u"url1"_s << urlMessageRef;
    }
    {
        // Image
        Message imageMessageRef;
        imageMessageRef.setMessageId("CD2LsyS4dNbj6TTjQ"_ba);
        imageMessageRef.setAlias(u"aliasname"_s);
        imageMessageRef.setName(u"name"_s);
        imageMessageRef.setGroupable(false);
        imageMessageRef.setTimeStamp(1505399177844);
        imageMessageRef.setUsername(u"username"_s);
        imageMessageRef.setUserId("FwHz8w8PWWQRqrJjM"_ba);
        imageMessageRef.setMessageType(Message::NormalText);
        imageMessageRef.setUpdatedAt(1505399177846);
        imageMessageRef.setEditedAt(-1);
        imageMessageRef.setIsStarred(false);
        imageMessageRef.setRoomId("dBWXYy4nyBHn8Q7dv"_ba);

        MessageAttachment attImage;
        attImage.setTitle(u"Clipboard"_s);
        attImage.setLink(u"/file-upload/Kt7DBWPe7pnadXDQH/test file"_s);
        attImage.setImageUrlPreview(u"/file-upload/Kt7DBWPe7pnadXDQH/test file"_s);
        attImage.setMimeType(u"image/png"_s);
        attImage.setImageWidth(120);
        attImage.setImageHeight(120);
        attImage.setAttachmentType(MessageAttachment::AttachmentType::Image);
        attImage.setAttachmentSize(1829038);
        attImage.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attImage);
        attachments.setMessageAttachments(attachmentInfos);

        imageMessageRef.setAttachments(attachments);

        QTest::addRow("image") << u"image"_s << imageMessageRef;
        // TODO add Mentions
    }

    {
        // Video
        Message videoMessageRef;
        videoMessageRef.setMessageId("KCy2KiFYmS5NuakPm"_ba);
        videoMessageRef.setAlias(u"Laurent Montel"_s);
        videoMessageRef.setName(u"Laurent Montel"_s);
        videoMessageRef.setGroupable(false);
        videoMessageRef.setTimeStamp(1515580853098);
        videoMessageRef.setUsername(u"laurent"_s);
        videoMessageRef.setUserId("uKK39zoewTkdacidH"_ba);
        videoMessageRef.setMessageType(Message::NormalText);
        videoMessageRef.setUpdatedAt(1515580853102);
        videoMessageRef.setEditedAt(-1);
        videoMessageRef.setIsStarred(false);
        videoMessageRef.setRoomId("kGtPa6bu7xHrS5xz6"_ba);

        MessageAttachment attVideo;
        attVideo.setTitle(u"SampleVideo_1280x720_1mb.mp4"_s);
        attVideo.setLink(u"/file-upload/sLgmaWowyttg4d2ZD/SampleVideo_1280x720_1mb.mp4"_s);
        attVideo.setDescription(u"test"_s);
        attVideo.setAttachmentSize(1055736);
        attVideo.setAttachmentType(MessageAttachment::AttachmentType::Video);
        attVideo.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attVideo);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        videoMessageRef.setAttachments(attachments);

        QTest::addRow("video") << u"video"_s << videoMessageRef;
    }
    {
        // Audio
        Message audioMessageRef;
        audioMessageRef.setMessageId("AwDsjWKJaW2wCP2ht"_ba);
        audioMessageRef.setAlias(u"Laurent Montel"_s);
        audioMessageRef.setName(u"Laurent Montel"_s);
        audioMessageRef.setGroupable(false);
        audioMessageRef.setTimeStamp(1515588347098);
        audioMessageRef.setUsername(u"laurent"_s);
        audioMessageRef.setUserId("uKK39zoewTkdacidH"_ba);
        audioMessageRef.setMessageType(Message::NormalText);
        audioMessageRef.setUpdatedAt(1515588347102);
        audioMessageRef.setEditedAt(-1);
        audioMessageRef.setIsStarred(false);
        audioMessageRef.setRoomId("kGtPa6bu7xHrS5xz6"_ba);

        MessageAttachment attAudio;
        attAudio.setTitle(u"joint.wav"_s);
        attAudio.setLink(u"/file-upload/9E8YBGgq3H6GbASf3/joint.wav"_s);
        attAudio.setDescription(u"dfgsdfgsdfg sdfgd dfsg sfd g"_s);
        attAudio.setAttachmentType(MessageAttachment::AttachmentType::Audio);
        attAudio.setAttachmentSize(59217);
        attAudio.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attAudio);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        audioMessageRef.setAttachments(attachments);

        QTest::addRow("audio") << u"audio"_s << audioMessageRef;
    }
    {
        // Message Starred
        Message messageStarredRef;
        messageStarredRef.setMessageId("R6AkSJ7orEkYHNcZJ"_ba);
        messageStarredRef.setAlias(u"Laurent"_s);
        messageStarredRef.setName(u"Laurent"_s);
        messageStarredRef.setGroupable(false);
        messageStarredRef.setTimeStamp(1516711967347);
        messageStarredRef.setUsername(u"laurent"_s);
        messageStarredRef.setUserId("uKK39zoewTkdacidH"_ba);
        messageStarredRef.setMessageType(Message::NormalText);
        messageStarredRef.setUpdatedAt(1516721464681);
        messageStarredRef.setEditedAt(1516712012309);
        messageStarredRef.setEditedByUsername(u"laurent"_s);
        messageStarredRef.setRoomId("kGtPa6bu7xHrS5xz6"_ba);
        messageStarredRef.setIsStarred(true);
        messageStarredRef.setText(u"sefssssdfsefdsdfsd"_s);

        QTest::addRow("messagestarred") << u"messagestarred"_s << messageStarredRef;
    }
    {
        // Message Starred
        Message messagePinnedRef;
        messagePinnedRef.setMessageId("4eawfwyFnt7L8wTwE"_ba);
        messagePinnedRef.setAlias(u"Laurent"_s);
        messagePinnedRef.setName(u"Laurent Montel"_s);
        messagePinnedRef.setGroupable(false);
        messagePinnedRef.setTimeStamp(1550240990669);
        messagePinnedRef.setUsername(u"laurent"_s);
        messagePinnedRef.setUserId("uKK39zoewTkdacidH"_ba);
        messagePinnedRef.setMessageType(Message::NormalText);
        messagePinnedRef.setUpdatedAt(1550240994061);
        messagePinnedRef.setRoomId("zMHhMfsEPvKjgFuyE"_ba);
        messagePinnedRef.setIsStarred(false);
        MessagePinned pinned;
        pinned.setPinned(true);
        pinned.setPinnedBy(u"laurent"_s);
        messagePinnedRef.setMessagePinned(pinned);
        messagePinnedRef.setText(u"sdfgsdfg"_s);

        QTest::addRow("messagepinned") << u"messagepinned"_s << messagePinnedRef;
    }

    {
        // Message Remove Starred status
        Message messageStarredRemovedRef;
        messageStarredRemovedRef.setMessageId("R6AkSJ7orEkYHNcZJ"_ba);
        messageStarredRemovedRef.setAlias(u"Laurent"_s);
        messageStarredRemovedRef.setName(u"Laurent"_s);
        messageStarredRemovedRef.setGroupable(false);
        messageStarredRemovedRef.setTimeStamp(1516711967347);
        messageStarredRemovedRef.setUsername(u"laurent"_s);
        messageStarredRemovedRef.setUserId("uKK39zoewTkdacidH"_ba);
        messageStarredRemovedRef.setMessageType(Message::NormalText);
        messageStarredRemovedRef.setUpdatedAt(1516721464681);
        messageStarredRemovedRef.setEditedAt(1516712012309);
        messageStarredRemovedRef.setEditedByUsername(u"laurent"_s);
        messageStarredRemovedRef.setRoomId("kGtPa6bu7xHrS5xz6"_ba);
        messageStarredRemovedRef.setIsStarred(false);
        messageStarredRemovedRef.setText(u"sefssssdfsefdsdfsd"_s);

        QTest::addRow("messageremovedstarred") << u"messageremovedstarred"_s << messageStarredRemovedRef;
    }
    {
        // messageattachmentfile
        Message messageAttachmentFileRef;
        messageAttachmentFileRef.setMessageId("messageid"_ba);
        messageAttachmentFileRef.setAlias(u"Laurent"_s);
        messageAttachmentFileRef.setName(u"Laurent Montel"_s);
        messageAttachmentFileRef.setGroupable(false);
        messageAttachmentFileRef.setTimeStamp(1520540888833);
        messageAttachmentFileRef.setUsername(u"laurent"_s);
        messageAttachmentFileRef.setUserId("39zoewTkdacidH"_ba);
        messageAttachmentFileRef.setMessageType(Message::NormalText);
        messageAttachmentFileRef.setUpdatedAt(1520540888836);
        messageAttachmentFileRef.setEditedAt(-1);
        messageAttachmentFileRef.setRoomId("Pa6bu7xHrS5xz6"_ba);
        messageAttachmentFileRef.setIsStarred(false);

        MessageAttachment fileAttachment;
        fileAttachment.setTitle(u"webkit.txt"_s);
        fileAttachment.setLink(u"/file-upload/tmqfdJTCmJ3oqG/webkit.txt"_s);
        fileAttachment.setDescription(u"description"_s);
        fileAttachment.setAttachmentType(MessageAttachment::AttachmentType::File);
        fileAttachment.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(fileAttachment);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        messageAttachmentFileRef.setAttachments(attachments);

        QTest::addRow("messageattachmentfile") << u"messageattachmentfile"_s << messageAttachmentFileRef;
    }
}

void MessageTest::shouldParseMessage()
{
    QFETCH(QString, name);
    QFETCH(Message, expectedMessage);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + name + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject obj = doc.object();
    Message originalMessage;
    originalMessage.parseMessage(obj, false, nullptr);
    const bool messageIsEqual = (originalMessage == expectedMessage);
    if (!messageIsEqual) {
        qDebug() << "originalMessage " << originalMessage;
        qDebug() << "ExpectedMessage " << expectedMessage;
    }
    QVERIFY(messageIsEqual);
}

void MessageTest::shouldSerializeData()
{
    {
        // Test discussionId
        Message input;

        input.setMessageId("ff"_ba);
        input.setRoomId("room1"_ba);
        input.setText(u"message1"_s);
        input.setTimeStamp(42);
        input.setUsername(u"user1"_s);
        input.setUserId("userid1"_ba);
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(u"editeduser1"_s);
        input.setAlias(u"ali"_s);
        input.setAvatar(u"avatar1"_s);
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(u"Local !!!!"_s);
        input.setParseUrls(true);
        input.setRole(u"leader"_s);
        input.setMessageType(Message::MessageType::NormalText);
        input.setDiscussionRoomId("discussion111"_ba);
        input.setDiscussionCount(90);
        input.setDiscussionLastMessage(985);
        MessageTranslations l;
        l.setTranslatedString({{u"foo"_s, u"bla"_s}});
        input.setMessageTranslation(l);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        const QByteArray ba = Message::serialize(input);
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());

        const bool compare = (input == output);
        if (!compare) {
            qDebug() << "input: " << input;
            qDebug() << "output: " << output << QCborValue::fromCbor(ba).toMap().toJsonObject();
        }

        QCOMPARE(input, output);

        QVERIFY(output.wasEdited());
    }
    {
        Message input;

        input.setMessageId("ff"_ba);
        input.setRoomId("room1"_ba);
        input.setText(u"message1"_s);
        input.setTimeStamp(42);
        input.setUsername(u"user1"_s);
        input.setUserId("userid1"_ba);
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(u"editeduser1"_s);
        input.setAlias(u"ali"_s);
        input.setAvatar(u"avatar1"_s);
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(u"leader"_s);
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        QList<MessageAttachment> lstAttachement;
        MessageAttachment attachment;
        attachment.setDescription(u"foo1"_s);
        attachment.setTitle(u"foo2"_s);
        attachment.setLink(u"foo3"_s);
        attachment.generateTitle();
        lstAttachement.append(std::move(attachment));
        MessageAttachment attachment2;
        attachment2.setDescription(u"foo5"_s);
        attachment2.setTitle(u"foo6"_s);
        attachment2.setLink(u"foo7"_s);
        attachment2.generateTitle();
        lstAttachement.append(std::move(attachment2));

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(lstAttachement);
        attachments.setMessageAttachments(attachmentInfos);

        input.setAttachments(attachments);

        QList<MessageUrl> lstUrls;
        MessageUrl url1;
        url1.setUrl(u"foo1"_s);
        url1.setPageTitle(u"foo2"_s);
        url1.generateMessageUrlInfo();
        url1.setUrlId("ff_0"_ba);
        lstUrls.append(std::move(url1));
        MessageUrl url2;
        url2.setUrl(u"foo5"_s);
        url2.setPageTitle(u"foo6"_s);
        url2.generateMessageUrlInfo();
        url2.setUrlId("ff_1"_ba);
        lstUrls.append(std::move(url2));

        MessageUrls u;
        u.setMessageUrls(lstUrls);
        input.setUrls(u);

        const QByteArray ba = Message::serialize(input);
        qDebug() << " ba " << ba;
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        QCOMPARE(input, output);
        // TODO add Mentions

        QVERIFY(output.wasEdited());
    }

    {
        // Test thread messageId
        Message input;

        input.setMessageId("ff"_ba);
        input.setRoomId("room1"_ba);
        input.setText(u"message1"_s);
        input.setTimeStamp(42);
        input.setUsername(u"user1"_s);
        input.setUserId("userid1"_ba);
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(u"editeduser1"_s);
        input.setAlias(u"ali"_s);
        input.setAvatar(u"avatar1"_s);
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(u"Local !!!!"_s);
        input.setParseUrls(true);
        input.setRole(u"leader"_s);
        input.setMessageType(Message::MessageType::NormalText);
        input.setThreadMessageId("thread111"_ba);
        input.setThreadCount(56);
        input.setThreadLastMessage(92);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        const QByteArray ba = Message::serialize(input);
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        QCOMPARE(input, output);

        QVERIFY(output.wasEdited());
    }

    {
        Message input;

        input.setMessageId("ff"_ba);
        input.setRoomId("room1"_ba);
        input.setText(u"message1"_s);
        input.setTimeStamp(42);
        input.setUsername(u"user1"_s);
        input.setUserId("userid1"_ba);
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(u"editeduser1"_s);
        input.setAlias(u"ali"_s);
        input.setAvatar(u"avatar1"_s);
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(u"leader"_s);
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        // Attachments
        QList<MessageAttachment> lstAttachement;
        {
            MessageAttachment attachment;
            attachment.setDescription(u"foo1"_s);
            attachment.setTitle(u"foo2"_s);
            attachment.setLink(u"foo3"_s);
            attachment.generateTitle();
            lstAttachement.append(std::move(attachment));
        }
        MessageAttachment attachment2;
        {
            attachment2.setDescription(u"foo5"_s);
            attachment2.setTitle(u"foo6"_s);
            attachment2.setLink(u"foo7"_s);
            attachment2.generateTitle();
            lstAttachement.append(std::move(attachment2));
        }
        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(lstAttachement);
        attachments.setMessageAttachments(attachmentInfos);

        input.setAttachments(attachments);
        // Urls
        QList<MessageUrl> lstUrls;
#if 0 // TODO FIXME
        {
            MessageUrl url1;
            url1.setUrl(u"foo1"_s);
            url1.setPageTitle(u"foo2"_s);
            url1.generateMessageUrlInfo();
            lstUrls.append(std::move(url1));
        }
        {
            MessageUrl url2;
            url2.setUrl(u"foo5"_s);
            url2.setPageTitle(u"foo6"_s);
            url2.generateMessageUrlInfo();
            lstUrls.append(std::move(url2));
        }
#endif

        MessageUrls u;
        u.setMessageUrls(lstUrls);
        input.setUrls(u);

        // Reactions
        QList<Reaction> reacts;
        {
            Reaction a;
            a.setUserNames({u"bla"_s, u"foo"_s});
            a.setReactionName(u":)"_s);
            reacts.append(std::move(a));
        }
        {
            Reaction a;
            a.setUserNames({u"bli"_s, u"aa"_s});
            a.setReactionName(u":foo:"_s);
            reacts.append(std::move(a));
        }

        Reactions reactions;
        reactions.setReactions(reacts);
        input.setReactions(reactions);

        // Message Translation
        MessageTranslations l;
        l.setTranslatedString({{u"foo"_s, u"bla"_s}});
        input.setMessageTranslation(l);

        // LocalTranslation
        input.setLocalTranslation(u"message fr"_s);

        // Mention
        QMap<QString, QByteArray> mentions;
        mentions.insert(u"bla1"_s, "foo1"_ba);
        mentions.insert(u"bla2"_s, "foo2"_ba);
        mentions.insert(u"bla3"_s, "foo3"_ba);
        mentions.insert(u"bla4"_s, "foo4"_ba);
        input.setMentions(mentions);

        // Channels
        Channels channels;
        QList<Channels::ChannelInfo> lst;
        {
            Channels::ChannelInfo info;
            info.fname = u"fnamechannel1"_s;
            info.name = u"channel1"_s;
            info.identifier = "foo-1"_ba;
            lst.append(std::move(info));
        }
        {
            Channels::ChannelInfo info;
            info.fname = u"fnamechannel2"_s;
            info.name = u"channel2"_s;
            info.identifier = "foo-2"_ba;
            lst.append(std::move(info));
        }
        {
            Channels::ChannelInfo info;
            info.fname = u"fnamechannel3"_s;
            info.name = u"channel3"_s;
            info.identifier = "foo-3"_ba;
            lst.append(std::move(info));
        }
        channels.setChannels(lst);
        input.setChannels(channels);

        // Starred
        MessageStarred starred;
        starred.setIsStarred(true);
        input.setMessageStarred(starred);

        // MessagePinned
        MessagePinned pinned;
        pinned.setPinned(true);
        pinned.setPinnedBy(u"kla"_s);
        input.setMessagePinned(pinned);

        // Blocks
        Blocks blocks;
        QList<Block> blockInfos;
        {
            Block b;
            b.setBlockId(u"block-id1"_s);
            b.setAppId(u"appid-1"_s);
            b.setBlockType(Block::BlockType::VideoConf);
            blockInfos.append(b);
        }
        {
            Block b;
            b.setBlockId(u"block-id2"_s);
            b.setAppId(u"appid-2"_s);
            b.setBlockType(Block::BlockType::VideoConf);
            blockInfos.append(b);
        }
        blocks.setBlocks(blockInfos);
        input.setBlocks(blocks);

        // Replies
        Replies replies;
        const QList<QByteArray> bareplies({QByteArrayLiteral("reply1"), "reply2"_ba});
        replies.setReplies(bareplies);
        input.setReplies(replies);

        // Discussion
        input.setDiscussionRoomId("discussion1"_ba);
        input.setDiscussionCount(55);
        input.setDiscussionLastMessage(666);

        // Thread
        input.setThreadMessageId("thread1"_ba);
        input.setThreadLastMessage(7777);
        input.setThreadCount(4);
        const QByteArray ba = Message::serialize(input);
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        const bool compare = (input == output);
        if (!compare) {
            qDebug() << "input: " << input;
            qDebug() << "output: " << output << QCborValue::fromCbor(ba).toMap().toJsonObject();
        }
        QCOMPARE(input, output);
        QVERIFY(output.wasEdited());
    }

    {
        // Test local translation
        Message input;

        input.setMessageId("ff"_ba);
        input.setRoomId("room1"_ba);
        input.setText(u"message1"_s);
        input.setTimeStamp(42);
        input.setUsername(u"user1"_s);
        input.setUserId("userid1"_ba);
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(u"editeduser1"_s);
        input.setAlias(u"ali"_s);
        input.setAvatar(u"avatar1"_s);
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(u"Local !!!!"_s);
        input.setParseUrls(true);
        input.setRole(u"leader"_s);
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        const QByteArray ba = Message::serialize(input);
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        QCOMPARE(input, output);

        QVERIFY(output.wasEdited());
    }
}

void MessageTest::shouldParseJsonMessage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow("empty") << u"empty"_s;
    QTest::newRow("standardmessage") << u"standardmessage"_s;
    QTest::newRow("attachmentimage") << u"attachmentimage"_s;
    QTest::newRow("attachment-author") << u"attachment-author"_s;
    QTest::newRow("message-url") << u"message-url"_s;
    QTest::newRow("message-url-meta-info") << u"message-url-meta-info"_s;
    QTest::newRow("message-reactions") << u"message-reactions"_s;
    QTest::newRow("animatedimage") << u"animatedimage"_s;
    QTest::newRow("emojimessage") << u"emojimessage"_s;
    QTest::newRow("attachmentaudio") << u"attachmentaudio"_s;
    QTest::newRow("attachmentimage-preview") << u"attachmentimage-preview"_s;
    QTest::newRow("message-attachment-link") << u"message-attachment-link"_s;
    // TODO fix them
    QTest::newRow("message-bug-435718") << u"message-bug-435718"_s;
    QTest::newRow("message-bug-435718-2") << u"message-bug-435718-2"_s;
    QTest::newRow("message-bug-435718-3") << u"message-bug-435718-3"_s;
    QTest::newRow("message-channel-mentions") << u"message-channel-mentions"_s;
}

void MessageTest::shouldParseJsonMessage()
{
    QFETCH(QString, fileName);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/messages/"_L1 + fileName + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields, false, nullptr);
    // qDebug() << " fields"<<fields;

    const QByteArray ba = Message::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/messages/"_s, jsonIndented, fileName);

    Message m = Message::deserialize(docSerialized.object());
    bool compareMessage = (r == m);
    if (!compareMessage) {
        qDebug() << "loaded message" << r;
        qDebug() << "fromJson " << m;
    }
    QVERIFY(compareMessage);
}

void MessageTest::shouldUpdateJsonMessage_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("fileNameupdate");
    QTest::newRow("standardmessage") << u"standardmessage"_s << QStringList();
    QTest::newRow("message1-init") << u"message1-init"_s << QStringList();
    QTest::newRow("message1") << u"message1"_s << (QStringList() << u"message1-updated"_s);
    QTest::newRow("message2") << u"message2"_s << (QStringList() << u"message2-updated"_s << u"message2-updated-stared"_s);
    QTest::newRow("message3") << u"message3"_s << (QStringList() << u"message3-updated"_s);
    // TODO add more !
}

void MessageTest::shouldUpdateJsonMessage()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, fileNameupdate);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/messages-updated/"_L1 + fileNameinit + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields, false, nullptr);

    for (const QString &updateFile : fileNameupdate) {
        const QString originalUpdateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/messages-updated/"_L1 + updateFile + ".json"_L1;
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.parseMessage(fields, false, nullptr);
    }

    // qDebug() << " fields"<<fields;
    const QByteArray ba = Message::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/messages-updated/"_s, jsonIndented, fileNameinit);

    Message m = Message::deserialize(docSerialized.object());
    const bool compareMessage = (r == m);
    if (!compareMessage) {
        qDebug() << "loaded message" << r;
        qDebug() << "fromJson " << m;
    }
    QVERIFY(compareMessage);
}

#include "moc_messagetest.cpp"
