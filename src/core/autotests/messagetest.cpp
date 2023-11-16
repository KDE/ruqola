/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetest.h"
#include "messages/message.h"
#include "ruqola_autotest_helper.h"
#include <QCborMap>
#include <QCborValue>
#include <QJsonDocument>

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
    QVERIFY(m.blocks().isEmpty());
    QVERIFY(m.attachments().isEmpty());
    QCOMPARE(m.discussionCount(), 0);
    QVERIFY(!m.goToMessageBackgroundColor().isValid());
}

// TODO add check for default value ???

void MessageTest::shouldParseMessage_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Message>("expectedMessage");

    {
        Message firstMessageRef;
        firstMessageRef.setMessageId(QStringLiteral("blaid"));
        firstMessageRef.setAlias(QStringLiteral("aliasname"));
        firstMessageRef.setGroupable(false);
        firstMessageRef.setTimeStamp(1504509615057);
        firstMessageRef.setUsername(QStringLiteral("foo.ff"));
        firstMessageRef.setName(QStringLiteral("newname"));
        firstMessageRef.setUserId(QStringLiteral("qt9uNY9FxyL5QT5no"));
        firstMessageRef.setMessageType(Message::NormalText);
        firstMessageRef.setUpdatedAt(1504509615063);
        firstMessageRef.setEditedAt(-1);
        firstMessageRef.setIsStarred(false);
        firstMessageRef.setRoomId(QStringLiteral("qt9uNY9FxyL5QT5nouKK39zoewTkdacidH"));
        MessageAttachment att;
        att.setLink(QStringLiteral("/file-upload/3zfLR3Cjr8YnvD6cS/dd.pdf"));
        att.setTitle(QStringLiteral("dd.pdf"));
        att.setAttachmentType(MessageAttachment::File);
        firstMessageRef.setAttachments({att});
        QTest::addRow("first") << QStringLiteral("first") << firstMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId(QStringLiteral("9kHnbbjbHKHjXXQp7"));
        urlMessageRef.setAlias(QStringLiteral("alias"));
        urlMessageRef.setGroupable(false);
        urlMessageRef.setName(QStringLiteral("name"));
        urlMessageRef.setTimeStamp(1504596899771);
        urlMessageRef.setUsername(QStringLiteral("username"));
        urlMessageRef.setUserId(QStringLiteral("bjBueJtHsHQECdkmH"));
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1504596901803);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setIsStarred(false);
        urlMessageRef.setRoomId(QStringLiteral("dBWXYy4nyBHn8Q7dv"));
        urlMessageRef.setText(QStringLiteral("https://foo.com/event/whoa"));
        MessageUrl url;
        url.setUrl(QStringLiteral("https://foo.com/event/C8CT"));
        url.setPageTitle(QStringLiteral(" Trainers Panel I"));
        url.setDescription(QStringLiteral("Description"));
        urlMessageRef.setUrls({url});
        QTest::addRow("url") << QStringLiteral("url") << urlMessageRef;
    }
    {
        // Image
        Message imageMessageRef;
        imageMessageRef.setMessageId(QStringLiteral("CD2LsyS4dNbj6TTjQ"));
        imageMessageRef.setAlias(QStringLiteral("aliasname"));
        imageMessageRef.setName(QStringLiteral("name"));
        imageMessageRef.setGroupable(false);
        imageMessageRef.setTimeStamp(1505399177844);
        imageMessageRef.setUsername(QStringLiteral("username"));
        imageMessageRef.setUserId(QStringLiteral("FwHz8w8PWWQRqrJjM"));
        imageMessageRef.setMessageType(Message::NormalText);
        imageMessageRef.setUpdatedAt(1505399177846);
        imageMessageRef.setEditedAt(-1);
        imageMessageRef.setIsStarred(false);
        imageMessageRef.setRoomId(QStringLiteral("dBWXYy4nyBHn8Q7dv"));

        MessageAttachment attImage;
        attImage.setTitle(QStringLiteral("Clipboard"));
        attImage.setLink(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
        attImage.setImageUrlPreview(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
        attImage.setMimeType(QStringLiteral("image/png"));
        attImage.setImageWidth(120);
        attImage.setImageHeight(120);
        attImage.setAttachmentType(MessageAttachment::Image);
        imageMessageRef.setAttachments({attImage});

        QTest::addRow("image") << QStringLiteral("image") << imageMessageRef;
        // TODO add Mentions
    }

    {
        // Video
        Message videoMessageRef;
        videoMessageRef.setMessageId(QStringLiteral("KCy2KiFYmS5NuakPm"));
        videoMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        videoMessageRef.setName(QStringLiteral("Laurent Montel"));
        videoMessageRef.setGroupable(false);
        videoMessageRef.setTimeStamp(1515580853098);
        videoMessageRef.setUsername(QStringLiteral("laurent"));
        videoMessageRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        videoMessageRef.setMessageType(Message::NormalText);
        videoMessageRef.setUpdatedAt(1515580853102);
        videoMessageRef.setEditedAt(-1);
        videoMessageRef.setIsStarred(false);
        videoMessageRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attVideo;
        attVideo.setTitle(QStringLiteral("SampleVideo_1280x720_1mb.mp4"));
        attVideo.setLink(QStringLiteral("/file-upload/sLgmaWowyttg4d2ZD/SampleVideo_1280x720_1mb.mp4"));
        attVideo.setDescription(QStringLiteral("test"));
        attVideo.setAttachmentType(MessageAttachment::Video);
        // Add video size/video type etc.
        videoMessageRef.setAttachments({attVideo});

        QTest::addRow("video") << QStringLiteral("video") << videoMessageRef;
    }
    {
        // Audio
        Message audioMessageRef;
        audioMessageRef.setMessageId(QStringLiteral("AwDsjWKJaW2wCP2ht"));
        audioMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        audioMessageRef.setName(QStringLiteral("Laurent Montel"));
        audioMessageRef.setGroupable(false);
        audioMessageRef.setTimeStamp(1515588347098);
        audioMessageRef.setUsername(QStringLiteral("laurent"));
        audioMessageRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        audioMessageRef.setMessageType(Message::NormalText);
        audioMessageRef.setUpdatedAt(1515588347102);
        audioMessageRef.setEditedAt(-1);
        audioMessageRef.setIsStarred(false);
        audioMessageRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attAudio;
        attAudio.setTitle(QStringLiteral("joint.wav"));
        attAudio.setLink(QStringLiteral("/file-upload/9E8YBGgq3H6GbASf3/joint.wav"));
        attAudio.setDescription(QStringLiteral("dfgsdfgsdfg sdfgd dfsg sfd g"));
        attAudio.setAttachmentType(MessageAttachment::Audio);
        // Add video size/video type etc.
        audioMessageRef.setAttachments({attAudio});

        QTest::addRow("audio") << QStringLiteral("audio") << audioMessageRef;
    }
    {
        // Message Starred
        Message messageStarredRef;
        messageStarredRef.setMessageId(QStringLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRef.setAlias(QStringLiteral("Laurent"));
        messageStarredRef.setName(QStringLiteral("Laurent"));
        messageStarredRef.setGroupable(false);
        messageStarredRef.setTimeStamp(1516711967347);
        messageStarredRef.setUsername(QStringLiteral("laurent"));
        messageStarredRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messageStarredRef.setMessageType(Message::NormalText);
        messageStarredRef.setUpdatedAt(1516721464681);
        messageStarredRef.setEditedAt(1516712012309);
        messageStarredRef.setEditedByUsername(QStringLiteral("laurent"));
        messageStarredRef.setEditedByUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messageStarredRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));
        messageStarredRef.setIsStarred(true);
        messageStarredRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messagestarred") << QStringLiteral("messagestarred") << messageStarredRef;
    }
    {
        // Message Starred
        Message messagePinnedRef;
        messagePinnedRef.setMessageId(QStringLiteral("4eawfwyFnt7L8wTwE"));
        messagePinnedRef.setAlias(QStringLiteral("Laurent"));
        messagePinnedRef.setName(QStringLiteral("Laurent Montel"));
        messagePinnedRef.setGroupable(false);
        messagePinnedRef.setTimeStamp(1550240990669);
        messagePinnedRef.setUsername(QStringLiteral("laurent"));
        messagePinnedRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messagePinnedRef.setMessageType(Message::NormalText);
        messagePinnedRef.setUpdatedAt(1550240994061);
        messagePinnedRef.setRoomId(QStringLiteral("zMHhMfsEPvKjgFuyE"));
        messagePinnedRef.setIsStarred(false);
        MessagePinned pinned;
        pinned.setPinned(true);
        pinned.setPinnedBy(QStringLiteral("laurent"));
        messagePinnedRef.setMessagePinned(pinned);
        messagePinnedRef.setText(QStringLiteral("sdfgsdfg"));

        QTest::addRow("messagepinned") << QStringLiteral("messagepinned") << messagePinnedRef;
    }

    {
        // Message Remove Starred status
        Message messageStarredRemovedRef;
        messageStarredRemovedRef.setMessageId(QStringLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRemovedRef.setAlias(QStringLiteral("Laurent"));
        messageStarredRemovedRef.setName(QStringLiteral("Laurent"));
        messageStarredRemovedRef.setGroupable(false);
        messageStarredRemovedRef.setTimeStamp(1516711967347);
        messageStarredRemovedRef.setUsername(QStringLiteral("laurent"));
        messageStarredRemovedRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messageStarredRemovedRef.setMessageType(Message::NormalText);
        messageStarredRemovedRef.setUpdatedAt(1516721464681);
        messageStarredRemovedRef.setEditedAt(1516712012309);
        messageStarredRemovedRef.setEditedByUsername(QStringLiteral("laurent"));
        messageStarredRemovedRef.setEditedByUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messageStarredRemovedRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));
        messageStarredRemovedRef.setIsStarred(false);
        messageStarredRemovedRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messageremovedstarred") << QStringLiteral("messageremovedstarred") << messageStarredRemovedRef;
    }
    {
        // messageattachmentfile
        Message messageAttachmentFileRef;
        messageAttachmentFileRef.setMessageId(QStringLiteral("messageid"));
        messageAttachmentFileRef.setAlias(QStringLiteral("Laurent"));
        messageAttachmentFileRef.setName(QStringLiteral("Laurent Montel"));
        messageAttachmentFileRef.setGroupable(false);
        messageAttachmentFileRef.setTimeStamp(1520540888833);
        messageAttachmentFileRef.setUsername(QStringLiteral("laurent"));
        messageAttachmentFileRef.setUserId(QStringLiteral("39zoewTkdacidH"));
        messageAttachmentFileRef.setMessageType(Message::NormalText);
        messageAttachmentFileRef.setUpdatedAt(1520540888836);
        messageAttachmentFileRef.setEditedAt(-1);
        messageAttachmentFileRef.setRoomId(QStringLiteral("Pa6bu7xHrS5xz6"));
        messageAttachmentFileRef.setIsStarred(false);

        MessageAttachment fileAttachment;
        fileAttachment.setTitle(QStringLiteral("webkit.txt"));
        fileAttachment.setLink(QStringLiteral("/file-upload/tmqfdJTCmJ3oqG/webkit.txt"));
        fileAttachment.setDescription(QStringLiteral("description"));
        fileAttachment.setAttachmentType(MessageAttachment::File);
        // Add video size/video type etc.
        messageAttachmentFileRef.setAttachments({fileAttachment});

        QTest::addRow("messageattachmentfile") << QStringLiteral("messageattachmentfile") << messageAttachmentFileRef;
    }
}

void MessageTest::shouldParseMessage()
{
    QFETCH(QString, name);
    QFETCH(Message, expectedMessage);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject obj = doc.object();
    Message originalMessage;
    originalMessage.parseMessage(obj);
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
        Message input;

        input.setMessageId(QStringLiteral("ff"));
        input.setRoomId(QStringLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QStringLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setEditedByUserId(QStringLiteral("editedbyid1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(QStringLiteral("type"));
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        QVector<MessageAttachment> lstAttachement;
        MessageAttachment attachment;
        attachment.setDescription(QStringLiteral("foo1"));
        attachment.setTitle(QStringLiteral("foo2"));
        attachment.setLink(QStringLiteral("foo3"));
        lstAttachement.append(std::move(attachment));
        MessageAttachment attachment2;
        attachment2.setDescription(QStringLiteral("foo5"));
        attachment2.setTitle(QStringLiteral("foo6"));
        attachment2.setLink(QStringLiteral("foo7"));
        lstAttachement.append(std::move(attachment2));
        input.setAttachments(lstAttachement);

        QVector<MessageUrl> lstUrls;
        MessageUrl url1;
        url1.setUrl(QStringLiteral("foo1"));
        url1.setPageTitle(QStringLiteral("foo2"));
        lstUrls.append(std::move(url1));
        MessageUrl url2;
        url2.setUrl(QStringLiteral("foo5"));
        url2.setPageTitle(QStringLiteral("foo6"));
        lstUrls.append(std::move(url2));
        input.setUrls(lstUrls);

        const QByteArray ba = Message::serialize(input);
        // Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        const Message output = Message::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        QCOMPARE(input, output);
        // TODO add Mentions

        QVERIFY(output.wasEdited());
    }
    {
        Message input;

        input.setMessageId(QStringLiteral("ff"));
        input.setRoomId(QStringLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QStringLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setEditedByUserId(QStringLiteral("editedbyid1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(QStringLiteral("type"));
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        // Attachments
        QVector<MessageAttachment> lstAttachement;
        {
            MessageAttachment attachment;
            attachment.setDescription(QStringLiteral("foo1"));
            attachment.setTitle(QStringLiteral("foo2"));
            attachment.setLink(QStringLiteral("foo3"));
            lstAttachement.append(std::move(attachment));
        }
        MessageAttachment attachment2;
        {
            attachment2.setDescription(QStringLiteral("foo5"));
            attachment2.setTitle(QStringLiteral("foo6"));
            attachment2.setLink(QStringLiteral("foo7"));
            lstAttachement.append(std::move(attachment2));
            input.setAttachments(lstAttachement);
        }

        // Urls
        QVector<MessageUrl> lstUrls;
        {
            MessageUrl url1;
            url1.setUrl(QStringLiteral("foo1"));
            url1.setPageTitle(QStringLiteral("foo2"));
            lstUrls.append(std::move(url1));
        }
        {
            MessageUrl url2;
            url2.setUrl(QStringLiteral("foo5"));
            url2.setPageTitle(QStringLiteral("foo6"));
            lstUrls.append(std::move(url2));
        }
        input.setUrls(lstUrls);

        // Reactions
        QVector<Reaction> reacts;
        {
            Reaction a;
            a.setUserNames({QStringLiteral("bla"), QStringLiteral("foo")});
            a.setReactionName(QStringLiteral(":)"));
            reacts.append(a);
        }
        {
            Reaction a;
            a.setUserNames({QStringLiteral("bli"), QStringLiteral("aa")});
            a.setReactionName(QStringLiteral(":foo:"));
            reacts.append(a);
        }

        Reactions reactions;
        reactions.setReactions(reacts);
        input.setReactions(reactions);

        // Message Translation
        MessageTranslation l;
        l.setTranslatedString({{QStringLiteral("foo"), QStringLiteral("bla")}});
        input.setMessageTranslation(l);

        // LocalTranslation
        input.setLocalTranslation(QStringLiteral("message fr"));

        // Mention
        QMap<QString, QString> mentions;
        mentions.insert(QStringLiteral("bla1"), QStringLiteral("foo1"));
        mentions.insert(QStringLiteral("bla2"), QStringLiteral("foo2"));
        mentions.insert(QStringLiteral("bla3"), QStringLiteral("foo3"));
        mentions.insert(QStringLiteral("bla4"), QStringLiteral("foo4"));
        input.setMentions(mentions);

        // Channels
        QMap<QString, QString> channels;
        channels.insert(QStringLiteral("channel1"), QStringLiteral("foo-1"));
        channels.insert(QStringLiteral("channel2"), QStringLiteral("foo-2"));
        channels.insert(QStringLiteral("channel3"), QStringLiteral("foo-3"));
        channels.insert(QStringLiteral("channel4"), QStringLiteral("foo-4"));
        input.setChannels(channels);

        // Starred
        MessageStarred starred;
        starred.setIsStarred(true);
        input.setMessageStarred(starred);

        // MessagePinned
        MessagePinned pinned;
        pinned.setPinned(true);
        pinned.setPinnedBy(QStringLiteral("kla"));
        input.setMessagePinned(pinned);

        // Blocks
        QVector<Block> blocks;
        {
            Block b;
            b.setBlockId(QStringLiteral("block-id1"));
            b.setAppId(QStringLiteral("appid-1"));
            b.setBlockType(Block::VideoConf);
            blocks.append(b);
        }
        {
            Block b;
            b.setBlockId(QStringLiteral("block-id2"));
            b.setAppId(QStringLiteral("appid-2"));
            b.setBlockType(Block::VideoConf);
            blocks.append(b);
        }
        input.setBlocks(blocks);

        // Replies
        input.setReplies({QStringLiteral("reply1"), QStringLiteral("reply2")});

        // Discussion
        input.setDiscussionRoomId(QStringLiteral("discussion1"));
        input.setDiscussionCount(55);
        input.setDiscussionLastMessage(666);

        // Thread
        input.setThreadMessageId(QStringLiteral("thread1"));
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
}

void MessageTest::shouldParseJsonMessage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::newRow("empty") << QStringLiteral("empty");
    QTest::newRow("standardmessage") << QStringLiteral("standardmessage");
    QTest::newRow("attachmentimage") << QStringLiteral("attachmentimage");
    QTest::newRow("attachment-author") << QStringLiteral("attachment-author");
    QTest::newRow("message-url") << QStringLiteral("message-url");
    QTest::newRow("message-url-meta-info") << QStringLiteral("message-url-meta-info");
    QTest::newRow("message-reactions") << QStringLiteral("message-reactions");
    QTest::newRow("animatedimage") << QStringLiteral("animatedimage");
    QTest::newRow("emojimessage") << QStringLiteral("emojimessage");
    QTest::newRow("attachmentaudio") << QStringLiteral("attachmentaudio");
    QTest::newRow("attachmentimage-preview") << QStringLiteral("attachmentimage-preview");
    QTest::newRow("message-attachment-link") << QStringLiteral("message-attachment-link");
    // TODO fix them
    QTest::newRow("message-bug-435718") << QStringLiteral("message-bug-435718");
    QTest::newRow("message-bug-435718-2") << QStringLiteral("message-bug-435718-2");
    QTest::newRow("message-bug-435718-3") << QStringLiteral("message-bug-435718-3");
    QTest::newRow("message-channel-mentions") << QStringLiteral("message-channel-mentions");
}

void MessageTest::shouldParseJsonMessage()
{
    QFETCH(QString, fileName);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/messages/") + fileName + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields);
    // qDebug() << " fields"<<fields;

    const QByteArray ba = Message::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(QStringLiteral("/messages/"), jsonIndented, fileName);

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
    QTest::newRow("standardmessage") << QStringLiteral("standardmessage") << QStringList();
    QTest::newRow("message1-init") << QStringLiteral("message1-init") << QStringList();
    QTest::newRow("message1") << QStringLiteral("message1") << (QStringList() << QStringLiteral("message1-updated"));
    QTest::newRow("message2") << QStringLiteral("message2")
                              << (QStringList() << QStringLiteral("message2-updated") << QStringLiteral("message2-updated-stared"));
    QTest::newRow("message3") << QStringLiteral("message3") << (QStringList() << QStringLiteral("message3-updated"));
    // TODO add more !
}

void MessageTest::shouldUpdateJsonMessage()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, fileNameupdate);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/messages-updated/") + fileNameinit + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields);

    for (const QString &updateFile : fileNameupdate) {
        const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/messages-updated/") + updateFile + QLatin1String(".json");
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.parseMessage(fields);
    }

    // qDebug() << " fields"<<fields;
    const QByteArray ba = Message::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(QStringLiteral("/messages-updated/"), jsonIndented, fileNameinit);

    Message m = Message::deserialize(docSerialized.object());
    const bool compareMessage = (r == m);
    if (!compareMessage) {
        qDebug() << "loaded message" << r;
        qDebug() << "fromJson " << m;
    }
    QVERIFY(compareMessage);
}

#include "moc_messagetest.cpp"
