/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperfiletest.h"

#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageDelegateHelperFileTest)

MessageDelegateHelperFileTest::MessageDelegateHelperFileTest(QObject *parent)
    : QObject(parent)
{
}

void MessageDelegateHelperFileTest::shouldLayoutMultipleFiles()
{
#if 0 // PORT IT
    // GIVEN a helper and a message with two attachments
    MessageAttachmentDelegateHelperFile helper;
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    Message message;

    // One attachment
    MessageAttachment msgAttach1;
    const QString title1 = QStringLiteral("This is the title");
    msgAttach1.setTitle(title1);
    const QString description1 = QStringLiteral("A description");
    msgAttach1.setDescription(description1);
    msgAttach1.setLink(QStringLiteral("http://www.kde.org"));

    // A second attachment
    MessageAttachment msgAttach2;
    const QString title2 = QStringLiteral("Another title");
    msgAttach2.setTitle(title2);
    const QString description2 = QStringLiteral("Another description");
    msgAttach2.setDescription(description2);
    msgAttach2.setLink(QStringLiteral("http://www.kdab.com"));

    message.setAttachements({msgAttach1, msgAttach2});

    // WHEN
    const QList<MessageAttachmentDelegateHelperFile::FileLayout> layouts = helper.doLayout(&message, option);

    // THEN
    QCOMPARE(layouts.count(), 2);
    const MessageAttachmentDelegateHelperFile::FileLayout layout1 = layouts.at(0);
    QCOMPARE(layout1.title, title1);
    QCOMPARE(layout1.description, description1);
    QCOMPARE(layout1.y, 0);
    QVERIFY(layout1.height > 0);
    const MessageAttachmentDelegateHelperFile::FileLayout layout2 = layouts.at(1);
    QCOMPARE(layout2.title, title2);
    QCOMPARE(layout2.description, description2);
    QVERIFY(layout2.y >= layout1.y + layout1.height);
#endif
}

#include "moc_messageattachmentdelegatehelperfiletest.cpp"
