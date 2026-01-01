/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmenttest.h"

#include "messages/messageattachment.h"
#include <QJsonObject>
#include <QStandardPaths>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(MessageAttachmentTest)
MessageAttachmentTest::MessageAttachmentTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void MessageAttachmentTest::shouldHaveDefaultValue()
{
    MessageAttachment attachment;
    QVERIFY(attachment.description().isEmpty());
    QVERIFY(attachment.title().isEmpty());
    QVERIFY(attachment.link().isEmpty());
    QVERIFY(!attachment.isValid());
    QVERIFY(attachment.color().isEmpty());
    QVERIFY(attachment.mimeType().isEmpty());
    QVERIFY(attachment.authorName().isEmpty());
    QCOMPARE(attachment.imageHeight(), -1);
    QCOMPARE(attachment.imageWidth(), -1);
    QVERIFY(!attachment.isAnimatedImage());
    QVERIFY(attachment.attachmentFields().isEmpty());
    QVERIFY(!attachment.collapsed());
    QVERIFY(!attachment.showAttachment());
    QVERIFY(!attachment.hasDescription());
    QCOMPARE(attachment.attachmentSize(), -1);
    QVERIFY(attachment.format().isEmpty());
}

void MessageAttachmentTest::shouldSerializeData()
{
    {
        MessageAttachment input;
        input.setColor(u"foo1"_s);
        input.setDescription(u"foo2"_s);
        input.setTitle(u"foo3"_s);
        input.setLink(u"foo4"_s);
        input.setImageHeight(53);
        input.setImageWidth(83);
        input.setAttachmentSize(454564);
        input.setAuthorName(u"auth"_s);
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setDescription(u"foo2"_s);
        input.setTitle(u"foo3"_s);
        input.setLink(u"foo4"_s);
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setColor(u"foo1"_s);
        input.setDescription(u"foo2"_s);
        input.setTitle(u"foo3"_s);
        input.setLink(u"foo4"_s);
        input.setAuthorName(u"auth"_s);
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }
}

void MessageAttachmentTest::shouldAllowToDownloadAttachment()
{
    MessageAttachment input;
    input.setColor(u"foo1"_s);
    input.setDescription(u"foo2"_s);
    input.setTitle(u"foo3"_s);
    input.setLink(u"http://www.kde.org"_s);
    input.setAuthorName(u"auth"_s);
    QVERIFY(!input.canDownloadAttachment());
    input.setLink(u"bla"_s);
    QVERIFY(input.canDownloadAttachment());
}

#include "moc_messageattachmenttest.cpp"
