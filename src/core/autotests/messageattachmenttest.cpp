/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "messageattachmenttest.h"
#include "messages/messageattachment.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(MessageAttachmentTest)

MessageAttachmentTest::MessageAttachmentTest(QObject *parent)
    : QObject(parent)
{
}

void MessageAttachmentTest::shouldHaveDefaultValue()
{
    MessageAttachment attachment;
    QVERIFY(attachment.description().isEmpty());
    QVERIFY(attachment.title().isEmpty());
    QVERIFY(attachment.link().isEmpty());
    QVERIFY(attachment.isEmpty());
    QVERIFY(attachment.color().isEmpty());
    QVERIFY(attachment.mimeType().isEmpty());
    QVERIFY(attachment.authorName().isEmpty());
    QCOMPARE(attachment.imageHeight(), -1);
    QCOMPARE(attachment.imageWidth(), -1);
    QVERIFY(!attachment.isAnimatedImage());
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
        input.setAuthorName(QStringLiteral("auth"));
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::fromJson(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setDescription(QStringLiteral("foo2"));
        input.setTitle(QStringLiteral("foo3"));
        input.setLink(QStringLiteral("foo4"));
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::fromJson(ba);
        QCOMPARE(input, output);
    }

    {
        MessageAttachment input;
        input.setColor(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo2"));
        input.setTitle(QStringLiteral("foo3"));
        input.setLink(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("auth"));
        const QJsonObject ba = MessageAttachment::serialize(input);
        const MessageAttachment output = MessageAttachment::fromJson(ba);
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
