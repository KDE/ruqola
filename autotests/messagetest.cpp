/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
#include "message.h"
#include <QTest>
#include <QJsonDocument>
QTEST_MAIN(MessageTest)
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
        firstMessageRef.setEditedAt(0);
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
        urlMessageRef.setEditedAt(0);
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
        imageMessageRef.setEditedAt(0);
        imageMessageRef.setRoomId(QStringLiteral("dBWXYy4nyBHn8Q7dv"));

        MessageAttachment attImage;
        attImage.setTitle(QStringLiteral("Clipboard"));
        attImage.setLink(QStringLiteral("/file-upload/Kt7DBWPe7pnadXDQH/test file"));
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
        videoMessageRef.setEditedAt(0);
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
        audioMessageRef.setEditedAt(0);
        audioMessageRef.setRoomId(QStringLiteral("kGtPa6bu7xHrS5xz6"));

        MessageAttachment attAudio;
        attAudio.setTitle(QStringLiteral("joint.wav"));
        attAudio.setLink(QStringLiteral("/file-upload/9E8YBGgq3H6GbASf3/joint.wav"));
        attAudio.setDescription(QStringLiteral("dfgsdfgsdfg sdfgd dfsg sfd g"));
        //Add video size/video type etc.
        audioMessageRef.setAttachements({attAudio});

        QTest::addRow("audio") << QStringLiteral("audio") << audioMessageRef;
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
    bool messageIsEqual = originalMessage.isEqual(expectedMessage);
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
    input.setMessageType(Message::MessageType::Audio);

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
    QVERIFY(input.isEqual(output));
    //TODO add Mentions
}
