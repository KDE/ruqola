/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetest.h"
#include "messages/message.h"
#include "ruqola_autotest_helper.h"
#include <QCborValue>
#include <QJsonDocument>
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
    // 14/03/2024 => size 816
    QCOMPARE(sizeof(Message), 448);
    QCOMPARE(m.messageStates(), Message::MessageStates(Message::MessageState::Groupable | Message::MessageState::Translated));
}

// TODO add check for default value ???

void MessageTest::shouldParseMessage_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Message>("expectedMessage");

    {
        Message firstMessageRef;
        firstMessageRef.setMessageId(QByteArrayLiteral("blaid"));
        firstMessageRef.setAlias(QStringLiteral("aliasname"));
        firstMessageRef.setGroupable(false);
        firstMessageRef.setTimeStamp(1504509615057);
        firstMessageRef.setUsername(QStringLiteral("foo.ff"));
        firstMessageRef.setName(QStringLiteral("newname"));
        firstMessageRef.setUserId(QByteArrayLiteral("qt9uNY9FxyL5QT5no"));
        firstMessageRef.setMessageType(Message::NormalText);
        firstMessageRef.setUpdatedAt(1504509615063);
        firstMessageRef.setEditedAt(-1);
        firstMessageRef.setIsStarred(false);
        firstMessageRef.setRoomId(QByteArrayLiteral("qt9uNY9FxyL5QT5nouKK39zoewTkdacidH"));
        MessageAttachment att;
        att.setLink(QStringLiteral("/file-upload/3zfLR3Cjr8YnvD6cS/dd.pdf"));
        att.setTitle(QStringLiteral("dd.pdf"));
        att.setAttachmentType(MessageAttachment::File);
        att.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(att);
        attachments.setMessageAttachments(attachmentInfos);

        firstMessageRef.setAttachments(attachments);
        QTest::addRow("first") << QStringLiteral("first") << firstMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId(QByteArrayLiteral("9kHnbbjbHKHjXXQp7"));
        urlMessageRef.setAlias(QStringLiteral("alias"));
        urlMessageRef.setGroupable(false);
        urlMessageRef.setName(QStringLiteral("name"));
        urlMessageRef.setTimeStamp(1504596899771);
        urlMessageRef.setUsername(QStringLiteral("username"));
        urlMessageRef.setUserId(QByteArrayLiteral("bjBueJtHsHQECdkmH"));
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1504596901803);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setIsStarred(false);
        urlMessageRef.setRoomId(QByteArrayLiteral("dBWXYy4nyBHn8Q7dv"));
        urlMessageRef.setText(QStringLiteral("https://foo.com/event/whoa"));
        MessageUrl url;
        url.setUrl(QStringLiteral("https://foo.com/event/C8CT"));
        url.setPageTitle(QStringLiteral("Trainers Panel I"));
        url.setDescription(QStringLiteral("View more about this event at CppCon 2017"));
        url.setImageUrl(QStringLiteral("http://foo.com/img/app-icon.png?1504596901"));
        url.setSiteUrl(QStringLiteral("https://foo.com/event/C8CT"));
        url.setUrlId(QByteArrayLiteral("9kHnbbjbHKHjXXQp7_0"));
        url.setImageWidth(200);
        url.setImageHeight(200);
        url.generateMessageUrlInfo();
        MessageUrls u;
        u.setMessageUrls({url});
        urlMessageRef.setUrls(u);

        QTest::addRow("url") << QStringLiteral("url") << urlMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId(QByteArrayLiteral("yZPPxBQ79M9jG5hS6"));
        urlMessageRef.setGroupable(false);
        urlMessageRef.setName(QStringLiteral("test1 test2"));
        urlMessageRef.setTimeStamp(1704484544725);
        urlMessageRef.setUsername(QStringLiteral("test1"));
        urlMessageRef.setUserId(QByteArrayLiteral("uKK39zoekdacidH"));
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1704484544886);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setIsStarred(false);
        urlMessageRef.setParseUrls(true);
        urlMessageRef.setRoomId(QByteArrayLiteral("wWTrQstz3mex5Mm"));
        urlMessageRef.setText(QStringLiteral("https://foo/pull/48/files#diff-142b45cf8aa373eab25fd7a81c45c11fb2bb07e4556b34162497ecb66b4aefb4R149"));
        MessageUrl url;
        url.setUrl(QStringLiteral("https://foo/pull/48/files#diff-142b45cf8aa373eab25fd7a81c45c11fb2bb07e4556b34162497ecb66b4aefb4R149"));
        url.setPageTitle(QStringLiteral("Deferred connection evaluation by blabla · Pull Request #48 · "));
        url.setDescription(QStringLiteral("Closes #23"));
        url.setImageUrl(QStringLiteral("https://avatar.foo.com/u/84974957?s=400&v=4"));
        url.setSiteUrl(QStringLiteral("https://foo/pull/48"));
        url.setSiteName(QStringLiteral("GitHub"));
        url.setImageWidth(1200);
        url.setImageHeight(600);
        url.setUrlId(QByteArrayLiteral("yZPPxBQ79M9jG5hS6_0"));
        url.generateMessageUrlInfo();
        MessageUrls u;
        u.setMessageUrls({url});
        urlMessageRef.setUrls(u);
        QTest::addRow("url1") << QStringLiteral("url1") << urlMessageRef;
    }
    {
        // Image
        Message imageMessageRef;
        imageMessageRef.setMessageId(QByteArrayLiteral("CD2LsyS4dNbj6TTjQ"));
        imageMessageRef.setAlias(QStringLiteral("aliasname"));
        imageMessageRef.setName(QStringLiteral("name"));
        imageMessageRef.setGroupable(false);
        imageMessageRef.setTimeStamp(1505399177844);
        imageMessageRef.setUsername(QStringLiteral("username"));
        imageMessageRef.setUserId(QByteArrayLiteral("FwHz8w8PWWQRqrJjM"));
        imageMessageRef.setMessageType(Message::NormalText);
        imageMessageRef.setUpdatedAt(1505399177846);
        imageMessageRef.setEditedAt(-1);
        imageMessageRef.setIsStarred(false);
        imageMessageRef.setRoomId(QByteArrayLiteral("dBWXYy4nyBHn8Q7dv"));

        MessageAttachment attImage;
        attImage.setTitle(QStringLiteral("Clipboard"));
        attImage.setLink(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
        attImage.setImageUrlPreview(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
        attImage.setMimeType(QStringLiteral("image/png"));
        attImage.setImageWidth(120);
        attImage.setImageHeight(120);
        attImage.setAttachmentType(MessageAttachment::Image);
        attImage.setAttachmentSize(1829038);
        attImage.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attImage);
        attachments.setMessageAttachments(attachmentInfos);

        imageMessageRef.setAttachments(attachments);

        QTest::addRow("image") << QStringLiteral("image") << imageMessageRef;
        // TODO add Mentions
    }

    {
        // Video
        Message videoMessageRef;
        videoMessageRef.setMessageId(QByteArrayLiteral("KCy2KiFYmS5NuakPm"));
        videoMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        videoMessageRef.setName(QStringLiteral("Laurent Montel"));
        videoMessageRef.setGroupable(false);
        videoMessageRef.setTimeStamp(1515580853098);
        videoMessageRef.setUsername(QStringLiteral("laurent"));
        videoMessageRef.setUserId(QByteArrayLiteral("uKK39zoewTkdacidH"));
        videoMessageRef.setMessageType(Message::NormalText);
        videoMessageRef.setUpdatedAt(1515580853102);
        videoMessageRef.setEditedAt(-1);
        videoMessageRef.setIsStarred(false);
        videoMessageRef.setRoomId(QByteArrayLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attVideo;
        attVideo.setTitle(QStringLiteral("SampleVideo_1280x720_1mb.mp4"));
        attVideo.setLink(QStringLiteral("/file-upload/sLgmaWowyttg4d2ZD/SampleVideo_1280x720_1mb.mp4"));
        attVideo.setDescription(QStringLiteral("test"));
        attVideo.setAttachmentSize(1055736);
        attVideo.setAttachmentType(MessageAttachment::Video);
        attVideo.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attVideo);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        videoMessageRef.setAttachments(attachments);

        QTest::addRow("video") << QStringLiteral("video") << videoMessageRef;
    }
    {
        // Audio
        Message audioMessageRef;
        audioMessageRef.setMessageId(QByteArrayLiteral("AwDsjWKJaW2wCP2ht"));
        audioMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        audioMessageRef.setName(QStringLiteral("Laurent Montel"));
        audioMessageRef.setGroupable(false);
        audioMessageRef.setTimeStamp(1515588347098);
        audioMessageRef.setUsername(QStringLiteral("laurent"));
        audioMessageRef.setUserId(QByteArrayLiteral("uKK39zoewTkdacidH"));
        audioMessageRef.setMessageType(Message::NormalText);
        audioMessageRef.setUpdatedAt(1515588347102);
        audioMessageRef.setEditedAt(-1);
        audioMessageRef.setIsStarred(false);
        audioMessageRef.setRoomId(QByteArrayLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attAudio;
        attAudio.setTitle(QStringLiteral("joint.wav"));
        attAudio.setLink(QStringLiteral("/file-upload/9E8YBGgq3H6GbASf3/joint.wav"));
        attAudio.setDescription(QStringLiteral("dfgsdfgsdfg sdfgd dfsg sfd g"));
        attAudio.setAttachmentType(MessageAttachment::Audio);
        attAudio.setAttachmentSize(59217);
        attAudio.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(attAudio);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        audioMessageRef.setAttachments(attachments);

        QTest::addRow("audio") << QStringLiteral("audio") << audioMessageRef;
    }
    {
        // Message Starred
        Message messageStarredRef;
        messageStarredRef.setMessageId(QByteArrayLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRef.setAlias(QStringLiteral("Laurent"));
        messageStarredRef.setName(QStringLiteral("Laurent"));
        messageStarredRef.setGroupable(false);
        messageStarredRef.setTimeStamp(1516711967347);
        messageStarredRef.setUsername(QStringLiteral("laurent"));
        messageStarredRef.setUserId(QByteArrayLiteral("uKK39zoewTkdacidH"));
        messageStarredRef.setMessageType(Message::NormalText);
        messageStarredRef.setUpdatedAt(1516721464681);
        messageStarredRef.setEditedAt(1516712012309);
        messageStarredRef.setEditedByUsername(QStringLiteral("laurent"));
        messageStarredRef.setRoomId(QByteArrayLiteral("kGtPa6bu7xHrS5xz6"));
        messageStarredRef.setIsStarred(true);
        messageStarredRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messagestarred") << QStringLiteral("messagestarred") << messageStarredRef;
    }
    {
        // Message Starred
        Message messagePinnedRef;
        messagePinnedRef.setMessageId(QByteArrayLiteral("4eawfwyFnt7L8wTwE"));
        messagePinnedRef.setAlias(QStringLiteral("Laurent"));
        messagePinnedRef.setName(QStringLiteral("Laurent Montel"));
        messagePinnedRef.setGroupable(false);
        messagePinnedRef.setTimeStamp(1550240990669);
        messagePinnedRef.setUsername(QStringLiteral("laurent"));
        messagePinnedRef.setUserId(QByteArrayLiteral("uKK39zoewTkdacidH"));
        messagePinnedRef.setMessageType(Message::NormalText);
        messagePinnedRef.setUpdatedAt(1550240994061);
        messagePinnedRef.setRoomId(QByteArrayLiteral("zMHhMfsEPvKjgFuyE"));
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
        messageStarredRemovedRef.setMessageId(QByteArrayLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRemovedRef.setAlias(QStringLiteral("Laurent"));
        messageStarredRemovedRef.setName(QStringLiteral("Laurent"));
        messageStarredRemovedRef.setGroupable(false);
        messageStarredRemovedRef.setTimeStamp(1516711967347);
        messageStarredRemovedRef.setUsername(QStringLiteral("laurent"));
        messageStarredRemovedRef.setUserId(QByteArrayLiteral("uKK39zoewTkdacidH"));
        messageStarredRemovedRef.setMessageType(Message::NormalText);
        messageStarredRemovedRef.setUpdatedAt(1516721464681);
        messageStarredRemovedRef.setEditedAt(1516712012309);
        messageStarredRemovedRef.setEditedByUsername(QStringLiteral("laurent"));
        messageStarredRemovedRef.setRoomId(QByteArrayLiteral("kGtPa6bu7xHrS5xz6"));
        messageStarredRemovedRef.setIsStarred(false);
        messageStarredRemovedRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messageremovedstarred") << QStringLiteral("messageremovedstarred") << messageStarredRemovedRef;
    }
    {
        // messageattachmentfile
        Message messageAttachmentFileRef;
        messageAttachmentFileRef.setMessageId(QByteArrayLiteral("messageid"));
        messageAttachmentFileRef.setAlias(QStringLiteral("Laurent"));
        messageAttachmentFileRef.setName(QStringLiteral("Laurent Montel"));
        messageAttachmentFileRef.setGroupable(false);
        messageAttachmentFileRef.setTimeStamp(1520540888833);
        messageAttachmentFileRef.setUsername(QStringLiteral("laurent"));
        messageAttachmentFileRef.setUserId(QByteArrayLiteral("39zoewTkdacidH"));
        messageAttachmentFileRef.setMessageType(Message::NormalText);
        messageAttachmentFileRef.setUpdatedAt(1520540888836);
        messageAttachmentFileRef.setEditedAt(-1);
        messageAttachmentFileRef.setRoomId(QByteArrayLiteral("Pa6bu7xHrS5xz6"));
        messageAttachmentFileRef.setIsStarred(false);

        MessageAttachment fileAttachment;
        fileAttachment.setTitle(QStringLiteral("webkit.txt"));
        fileAttachment.setLink(QStringLiteral("/file-upload/tmqfdJTCmJ3oqG/webkit.txt"));
        fileAttachment.setDescription(QStringLiteral("description"));
        fileAttachment.setAttachmentType(MessageAttachment::File);
        fileAttachment.generateTitle();

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(fileAttachment);
        attachments.setMessageAttachments(attachmentInfos);

        // Add video size/video type etc.
        messageAttachmentFileRef.setAttachments(attachments);

        QTest::addRow("messageattachmentfile") << QStringLiteral("messageattachmentfile") << messageAttachmentFileRef;
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

        input.setMessageId(QByteArrayLiteral("ff"));
        input.setRoomId(QByteArrayLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QByteArrayLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(QStringLiteral("Local !!!!"));
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        input.setDiscussionRoomId(QByteArrayLiteral("discussion111"));
        input.setDiscussionCount(90);
        input.setDiscussionLastMessage(985);
        MessageTranslation l;
        l.setTranslatedString({{QStringLiteral("foo"), QStringLiteral("bla")}});
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

        input.setMessageId(QByteArrayLiteral("ff"));
        input.setRoomId(QByteArrayLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QByteArrayLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        QList<MessageAttachment> lstAttachement;
        MessageAttachment attachment;
        attachment.setDescription(QStringLiteral("foo1"));
        attachment.setTitle(QStringLiteral("foo2"));
        attachment.setLink(QStringLiteral("foo3"));
        attachment.generateTitle();
        lstAttachement.append(std::move(attachment));
        MessageAttachment attachment2;
        attachment2.setDescription(QStringLiteral("foo5"));
        attachment2.setTitle(QStringLiteral("foo6"));
        attachment2.setLink(QStringLiteral("foo7"));
        attachment2.generateTitle();
        lstAttachement.append(std::move(attachment2));

        MessageAttachments attachments;
        QList<MessageAttachment> attachmentInfos;
        attachmentInfos.append(lstAttachement);
        attachments.setMessageAttachments(attachmentInfos);

        input.setAttachments(attachments);

        QList<MessageUrl> lstUrls;
        MessageUrl url1;
        url1.setUrl(QStringLiteral("foo1"));
        url1.setPageTitle(QStringLiteral("foo2"));
        url1.generateMessageUrlInfo();
        url1.setUrlId(QByteArrayLiteral("ff_0"));
        lstUrls.append(std::move(url1));
        MessageUrl url2;
        url2.setUrl(QStringLiteral("foo5"));
        url2.setPageTitle(QStringLiteral("foo6"));
        url2.generateMessageUrlInfo();
        url2.setUrlId(QByteArrayLiteral("ff_1"));
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

        input.setMessageId(QByteArrayLiteral("ff"));
        input.setRoomId(QByteArrayLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QByteArrayLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(QStringLiteral("Local !!!!"));
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        input.setThreadMessageId(QByteArrayLiteral("thread111"));
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

        input.setMessageId(QByteArrayLiteral("ff"));
        input.setRoomId(QByteArrayLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QByteArrayLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setGroupable(true);
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
        input.setMessageType(Message::MessageType::NormalText);
        // It will break as it's not supported yet
        input.setIsStarred(true);

        // Attachments
        QList<MessageAttachment> lstAttachement;
        {
            MessageAttachment attachment;
            attachment.setDescription(QStringLiteral("foo1"));
            attachment.setTitle(QStringLiteral("foo2"));
            attachment.setLink(QStringLiteral("foo3"));
            attachment.generateTitle();
            lstAttachement.append(std::move(attachment));
        }
        MessageAttachment attachment2;
        {
            attachment2.setDescription(QStringLiteral("foo5"));
            attachment2.setTitle(QStringLiteral("foo6"));
            attachment2.setLink(QStringLiteral("foo7"));
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
            url1.setUrl(QStringLiteral("foo1"));
            url1.setPageTitle(QStringLiteral("foo2"));
            url1.generateMessageUrlInfo();
            lstUrls.append(std::move(url1));
        }
        {
            MessageUrl url2;
            url2.setUrl(QStringLiteral("foo5"));
            url2.setPageTitle(QStringLiteral("foo6"));
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
            a.setUserNames({QStringLiteral("bla"), QStringLiteral("foo")});
            a.setReactionName(QStringLiteral(":)"));
            reacts.append(std::move(a));
        }
        {
            Reaction a;
            a.setUserNames({QStringLiteral("bli"), QStringLiteral("aa")});
            a.setReactionName(QStringLiteral(":foo:"));
            reacts.append(std::move(a));
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
        QMap<QString, QByteArray> mentions;
        mentions.insert(QStringLiteral("bla1"), QByteArrayLiteral("foo1"));
        mentions.insert(QStringLiteral("bla2"), QByteArrayLiteral("foo2"));
        mentions.insert(QStringLiteral("bla3"), QByteArrayLiteral("foo3"));
        mentions.insert(QStringLiteral("bla4"), QByteArrayLiteral("foo4"));
        input.setMentions(mentions);

        // Channels
        Channels channels;
        QList<Channels::ChannelInfo> lst;
        {
            Channels::ChannelInfo info;
            info.fname = QStringLiteral("fnamechannel1");
            info.name = QStringLiteral("channel1");
            info.identifier = QByteArrayLiteral("foo-1");
            lst.append(std::move(info));
        }
        {
            Channels::ChannelInfo info;
            info.fname = QStringLiteral("fnamechannel2");
            info.name = QStringLiteral("channel2");
            info.identifier = QByteArrayLiteral("foo-2");
            lst.append(std::move(info));
        }
        {
            Channels::ChannelInfo info;
            info.fname = QStringLiteral("fnamechannel3");
            info.name = QStringLiteral("channel3");
            info.identifier = QByteArrayLiteral("foo-3");
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
        pinned.setPinnedBy(QStringLiteral("kla"));
        input.setMessagePinned(pinned);

        // Blocks
        Blocks blocks;
        QList<Block> blockInfos;
        {
            Block b;
            b.setBlockId(QStringLiteral("block-id1"));
            b.setAppId(QStringLiteral("appid-1"));
            b.setBlockType(Block::VideoConf);
            blockInfos.append(b);
        }
        {
            Block b;
            b.setBlockId(QStringLiteral("block-id2"));
            b.setAppId(QStringLiteral("appid-2"));
            b.setBlockType(Block::VideoConf);
            blockInfos.append(b);
        }
        blocks.setBlocks(blockInfos);
        input.setBlocks(blocks);

        // Replies
        input.setReplies({QByteArrayLiteral("reply1"), QByteArrayLiteral("reply2")});

        // Discussion
        input.setDiscussionRoomId(QByteArrayLiteral("discussion1"));
        input.setDiscussionCount(55);
        input.setDiscussionLastMessage(666);

        // Thread
        input.setThreadMessageId(QByteArrayLiteral("thread1"));
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

        input.setMessageId(QByteArrayLiteral("ff"));
        input.setRoomId(QByteArrayLiteral("room1"));
        input.setText(QStringLiteral("message1"));
        input.setTimeStamp(42);
        input.setUsername(QStringLiteral("user1"));
        input.setUserId(QByteArrayLiteral("userid1"));
        input.setUpdatedAt(45);
        input.setEditedAt(89);
        input.setEditedByUsername(QStringLiteral("editeduser1"));
        input.setAlias(QStringLiteral("ali"));
        input.setAvatar(QStringLiteral("avatar1"));
        input.setSystemMessageType(SystemMessageTypeUtil::MessageDeleted);
        input.setLocalTranslation(QStringLiteral("Local !!!!"));
        input.setParseUrls(true);
        input.setRole(QStringLiteral("leader"));
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
