/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#include "messagetest.h"
#include "messages/message.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(MessageTest)
MessageTest::MessageTest(QObject *parent)
    : QObject(parent)
{
}

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
        firstMessageRef.setUserId(QStringLiteral("qt9uNY9FxyL5QT5no"));
        firstMessageRef.setMessageType(Message::File);
        firstMessageRef.setUpdatedAt(1504509615063);
        firstMessageRef.setEditedAt(-1);
        firstMessageRef.setStarred(false);
        firstMessageRef.setRoomId(QStringLiteral("qt9uNY9FxyL5QT5nouKK39zoewTkdacidH"));
        MessageAttachment att;
        att.setLink(QStringLiteral("/file-upload/3zfLR3Cjr8YnvD6cS/dd.pdf"));
        att.setTitle(QStringLiteral("dd.pdf"));
        firstMessageRef.setAttachements({att});
        QTest::addRow("first") << QStringLiteral("first") << firstMessageRef;
    }
    {
        Message urlMessageRef;
        urlMessageRef.setMessageId(QStringLiteral("9kHnbbjbHKHjXXQp7"));
        urlMessageRef.setAlias(QStringLiteral("alias"));
        urlMessageRef.setGroupable(false);
        urlMessageRef.setTimeStamp(1504596899771);
        urlMessageRef.setUsername(QStringLiteral("username"));
        urlMessageRef.setUserId(QStringLiteral("bjBueJtHsHQECdkmH"));
        urlMessageRef.setMessageType(Message::NormalText);
        urlMessageRef.setUpdatedAt(1504596901803);
        urlMessageRef.setEditedAt(-1);
        urlMessageRef.setStarred(false);
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
        //Image
        Message imageMessageRef;
        imageMessageRef.setMessageId(QStringLiteral("CD2LsyS4dNbj6TTjQ"));
        imageMessageRef.setAlias(QStringLiteral("aliasname"));
        imageMessageRef.setGroupable(false);
        imageMessageRef.setTimeStamp(1505399177844);
        imageMessageRef.setUsername(QStringLiteral("username"));
        imageMessageRef.setUserId(QStringLiteral("FwHz8w8PWWQRqrJjM"));
        imageMessageRef.setMessageType(Message::NormalText);
        imageMessageRef.setUpdatedAt(1505399177846);
        imageMessageRef.setEditedAt(-1);
        imageMessageRef.setStarred(false);
        imageMessageRef.setRoomId(QStringLiteral("dBWXYy4nyBHn8Q7dv"));

        MessageAttachment attImage;
        attImage.setTitle(QStringLiteral("Clipboard"));
        attImage.setLink(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
        attImage.setMimeType(QStringLiteral("image/png"));
        imageMessageRef.setAttachements({attImage});

        QTest::addRow("image") << QStringLiteral("image") << imageMessageRef;
        //TODO add Mentions
    }

    {
        //Video
        Message videoMessageRef;
        videoMessageRef.setMessageId(QStringLiteral("KCy2KiFYmS5NuakPm"));
        videoMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        videoMessageRef.setGroupable(false);
        videoMessageRef.setTimeStamp(1515580853098);
        videoMessageRef.setUsername(QStringLiteral("laurent"));
        videoMessageRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        videoMessageRef.setMessageType(Message::Video);
        videoMessageRef.setUpdatedAt(1515580853102);
        videoMessageRef.setEditedAt(-1);
        videoMessageRef.setStarred(false);
        videoMessageRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attVideo;
        attVideo.setTitle(QStringLiteral("SampleVideo_1280x720_1mb.mp4"));
        attVideo.setLink(QStringLiteral("/file-upload/sLgmaWowyttg4d2ZD/SampleVideo_1280x720_1mb.mp4"));
        attVideo.setDescription(QStringLiteral("test"));
        //Add video size/video type etc.
        videoMessageRef.setAttachements({attVideo});

        QTest::addRow("video") << QStringLiteral("video") << videoMessageRef;
    }
    {
        //Audio
        Message audioMessageRef;
        audioMessageRef.setMessageId(QStringLiteral("AwDsjWKJaW2wCP2ht"));
        audioMessageRef.setAlias(QStringLiteral("Laurent Montel"));
        audioMessageRef.setGroupable(false);
        audioMessageRef.setTimeStamp(1515588347098);
        audioMessageRef.setUsername(QStringLiteral("laurent"));
        audioMessageRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        audioMessageRef.setMessageType(Message::Audio);
        audioMessageRef.setUpdatedAt(1515588347102);
        audioMessageRef.setEditedAt(-1);
        audioMessageRef.setStarred(false);
        audioMessageRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attAudio;
        attAudio.setTitle(QStringLiteral("joint.wav"));
        attAudio.setLink(QStringLiteral("/file-upload/9E8YBGgq3H6GbASf3/joint.wav"));
        attAudio.setDescription(QStringLiteral("dfgsdfgsdfg sdfgd dfsg sfd g"));
        //Add video size/video type etc.
        audioMessageRef.setAttachements({attAudio});

        QTest::addRow("audio") << QStringLiteral("audio") << audioMessageRef;
    }
    {
        //Message Starred
        Message messageStarredRef;
        messageStarredRef.setMessageId(QStringLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRef.setAlias(QStringLiteral("Laurent"));
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
        messageStarredRef.setStarred(true);
        messageStarredRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messagestarred") << QStringLiteral("messagestarred") << messageStarredRef;
    }
    {
        //Message Starred
        Message messagePinnedRef;
        messagePinnedRef.setMessageId(QStringLiteral("4eawfwyFnt7L8wTwE"));
        messagePinnedRef.setAlias(QStringLiteral("Laurent"));
        messagePinnedRef.setGroupable(false);
        messagePinnedRef.setTimeStamp(1550240990669);
        messagePinnedRef.setUsername(QStringLiteral("laurent"));
        messagePinnedRef.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        messagePinnedRef.setMessageType(Message::NormalText);
        messagePinnedRef.setUpdatedAt(1550240994061);
        messagePinnedRef.setRoomId(QStringLiteral("zMHhMfsEPvKjgFuyE"));
        messagePinnedRef.setStarred(false);
        MessagePinned pinned;
        pinned.setPinned(true);
        messagePinnedRef.setMessagePinned(pinned);
        messagePinnedRef.setText(QStringLiteral("sdfgsdfg"));

        QTest::addRow("messagepinned") << QStringLiteral("messagepinned") << messagePinnedRef;
    }

    {
        //Message Remove Starred status
        Message messageStarredRemovedRef;
        messageStarredRemovedRef.setMessageId(QStringLiteral("R6AkSJ7orEkYHNcZJ"));
        messageStarredRemovedRef.setAlias(QStringLiteral("Laurent"));
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
        messageStarredRemovedRef.setStarred(false);
        messageStarredRemovedRef.setText(QStringLiteral("sefssssdfsefdsdfsd"));

        QTest::addRow("messageremovedstarred") << QStringLiteral("messageremovedstarred") << messageStarredRemovedRef;
    }
    {
        //messageattachmentfile
        Message messageAttachmentFileRef;
        messageAttachmentFileRef.setMessageId(QStringLiteral("messageid"));
        messageAttachmentFileRef.setAlias(QStringLiteral("Laurent"));
        messageAttachmentFileRef.setGroupable(false);
        messageAttachmentFileRef.setTimeStamp(1520540888833);
        messageAttachmentFileRef.setUsername(QStringLiteral("laurent"));
        messageAttachmentFileRef.setUserId(QStringLiteral("39zoewTkdacidH"));
        messageAttachmentFileRef.setMessageType(Message::File);
        messageAttachmentFileRef.setUpdatedAt(1520540888836);
        messageAttachmentFileRef.setEditedAt(-1);
        messageAttachmentFileRef.setRoomId(QStringLiteral("Pa6bu7xHrS5xz6"));
        messageAttachmentFileRef.setStarred(false);

        MessageAttachment fileAttachment;
        fileAttachment.setTitle(QStringLiteral("webkit.txt"));
        fileAttachment.setLink(QStringLiteral("/file-upload/tmqfdJTCmJ3oqG/webkit.txt"));
        fileAttachment.setDescription(QStringLiteral("description"));
        //Add video size/video type etc.
        messageAttachmentFileRef.setAttachements({fileAttachment});

        QTest::addRow("messageattachmentfile") << QStringLiteral("messageattachmentfile") << messageAttachmentFileRef;
    }
}

void MessageTest::shouldParseMessage()
{
    QFETCH(QString, name);
    QFETCH(Message, expectedMessage);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + name + QStringLiteral(".json");
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
        qDebug() << "originalMessage "<<originalMessage;
        qDebug() << "ExpectedMessage "<<expectedMessage;
    }
    QVERIFY(messageIsEqual);
}

void MessageTest::shouldSerializeData()
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
    input.setMessageType(Message::MessageType::Audio);
    //It will break as it's not supported yet
    input.setStarred(true);

    QVector<MessageAttachment> lstAttachement;
    MessageAttachment attachment;
    attachment.setDescription(QStringLiteral("foo1"));
    attachment.setTitle(QStringLiteral("foo2"));
    attachment.setLink(QStringLiteral("foo3"));
    lstAttachement.append(attachment);
    MessageAttachment attachment2;
    attachment2.setDescription(QStringLiteral("foo5"));
    attachment2.setTitle(QStringLiteral("foo6"));
    attachment2.setLink(QStringLiteral("foo7"));
    lstAttachement.append(attachment2);
    input.setAttachements(lstAttachement);

    QVector<MessageUrl> lstUrls;
    MessageUrl url1;
    url1.setUrl(QStringLiteral("foo1"));
    url1.setPageTitle(QStringLiteral("foo2"));
    lstUrls.append(url1);
    MessageUrl url2;
    url2.setUrl(QStringLiteral("foo5"));
    url2.setPageTitle(QStringLiteral("foo6"));
    lstUrls.append(url2);
    input.setUrls(lstUrls);

    const QByteArray ba = Message::serialize(input);
    Message output = Message::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    QCOMPARE(input, output);
    //TODO add Mentions
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
}

void MessageTest::shouldParseJsonMessage()
{
    QFETCH(QString, fileName);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/messages/") + fileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields);
    //qDebug() << " fields"<<fields;

    const QByteArray ba = Message::serialize(r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/messages/"), jsonIndented, fileName);

    Message m = Message::fromJSon(docSerialized.object());
    bool compareMessage = (r == m);
    if (!compareMessage) {
        qDebug() << "loaded message" << r;
        qDebug() << "fromJson " << m;
    }
    QEXPECT_FAIL("attachmentimage", "Message doesn't save/load mentions", Continue);
    QVERIFY(compareMessage);
}

void MessageTest::shouldUpdateJsonMessage_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("fileNameupdate");
    QTest::newRow("standardmessage") << QStringLiteral("standardmessage") << QStringList();
    QTest::newRow("message1-init") << QStringLiteral("message1-init") << QStringList();
    QTest::newRow("message1") << QStringLiteral("message1") << (QStringList() << QStringLiteral("message1-updated"));
    QTest::newRow("message2") << QStringLiteral("message2") << (QStringList() << QStringLiteral("message2-updated") << QStringLiteral("message2-updated-stared"));
    QTest::newRow("message3") << QStringLiteral("message3") << (QStringList() << QStringLiteral("message3-updated"));
    //TODO add more !
}

void MessageTest::shouldUpdateJsonMessage()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, fileNameupdate);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/messages-updated/") + fileNameinit + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Message r;
    r.parseMessage(fields);

    for (const QString &updateFile : fileNameupdate) {
        const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/messages-updated/") + updateFile + QStringLiteral(".json");
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.parseMessage(fields);
    }

    //qDebug() << " fields"<<fields;
    const QByteArray ba = Message::serialize(r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/messages-updated/"), jsonIndented, fileNameinit);

    Message m = Message::fromJSon(docSerialized.object());
    const bool compareMessage = (r == m);
    if (!compareMessage) {
        qDebug() << "loaded message" << r;
        qDebug() << "fromJson " << m;
    }
    QVERIFY(compareMessage);
}
