/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmenttest.h"

#include "messages/messageattachment.h"
#include "messages/messageattachmentfield.h"
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
    const MessageAttachment attachment;
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
    QVERIFY(!attachment.messageAttachmentActions().isValid());
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

void MessageAttachmentTest::shouldEscapeAttachmentFieldsText()
{
    MessageAttachment input;
    QVERIFY(input.attachmentFieldsText().isEmpty());

    MessageAttachmentField field;
    field.setTitle(u"<b>title</b>"_s);
    field.setValue(u"<i>value</i> & co"_s);
    input.setAttachmentFields({field});

    const QString text = input.attachmentFieldsText();
    QVERIFY(text.contains(u"<b>&lt;b&gt;title&lt;/b&gt;</b>"_s));
    QVERIFY(text.contains(u"&lt;i&gt;value&lt;/i&gt; &amp; co"_s));
    QVERIFY(!text.contains(u"<b>title</b>"_s));
    QVERIFY(!text.contains(u"<i>value</i>"_s));
}

#include "moc_messageattachmenttest.cpp"
