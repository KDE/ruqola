/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmenttest.h"
#include "messages/messageattachment.h"
#include <QJsonObject>
#include <QStandardPaths>
#include <QTest>
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
}

void MessageAttachmentTest::shouldSerializeData()
{
    {
        MessageAttachment input;
        input.setColor(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo2"));
        input.setTitle(QStringLiteral("foo3"));
        input.setLink(QStringLiteral("foo4"));
        input.setImageHeight(53);
        input.setImageWidth(83);
        input.setAttachmentSize(454564);
        input.setAuthorName(QStringLiteral("auth"));
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setDescription(QStringLiteral("foo2"));
        input.setTitle(QStringLiteral("foo3"));
        input.setLink(QStringLiteral("foo4"));
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setColor(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo2"));
        input.setTitle(QStringLiteral("foo3"));
        input.setLink(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("auth"));
        input.generateTitle();
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::deserialize(ba);
        QCOMPARE(input, output);
    }
}

void MessageAttachmentTest::shouldAllowToDownloadAttachment()
{
    MessageAttachment input;
    input.setColor(QStringLiteral("foo1"));
    input.setDescription(QStringLiteral("foo2"));
    input.setTitle(QStringLiteral("foo3"));
    input.setLink(QStringLiteral("http://www.kde.org"));
    input.setAuthorName(QStringLiteral("auth"));
    QVERIFY(!input.canDownloadAttachment());
    input.setLink(QStringLiteral("bla"));
    QVERIFY(input.canDownloadAttachment());
}

#include "moc_messageattachmenttest.cpp"
