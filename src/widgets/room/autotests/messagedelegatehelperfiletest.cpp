/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagedelegatehelperfiletest.h"
#include "room/messagedelegatehelperfile.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messages/message.h"
#include "messages/messageattachment.h"

#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageDelegateHelperFileTest)

MessageDelegateHelperFileTest::MessageDelegateHelperFileTest(QObject *parent)
    : QObject(parent)
{
}

void MessageDelegateHelperFileTest::shouldLayoutMultipleFiles()
{
    // GIVEN a helper and a message with two attachments
    MessageDelegateHelperFile helper;
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
    const QVector<MessageDelegateHelperFile::FileLayout> layouts = helper.doLayout(&message, option);

    // THEN
    QCOMPARE(layouts.count(), 2);
    const MessageDelegateHelperFile::FileLayout layout1 = layouts.at(0);
    QCOMPARE(layout1.title, title1);
    QCOMPARE(layout1.description, description1);
    QCOMPARE(layout1.y, 0);
    QVERIFY(layout1.height > 0);
    const MessageDelegateHelperFile::FileLayout layout2 = layouts.at(1);
    QCOMPARE(layout2.title, title2);
    QCOMPARE(layout2.description, description2);
    QVERIFY(layout2.y >= layout1.y + layout1.height);
}
