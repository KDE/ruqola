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

#include "messagedelegatehelperimagetest.h"
#include "room/delegate/messagedelegatehelperimage.h"
#include "managerdatapaths.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messages/message.h"
#include "messages/messageattachment.h"
#include "testdata.h"

#include <QStandardPaths>
#include <QStyleOptionViewItem>
#include <QTest>

QTEST_MAIN(MessageDelegateHelperImageTest)

MessageDelegateHelperImageTest::MessageDelegateHelperImageTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
    Ruqola::self()->rocketChatAccount()->setAccountName(QStringLiteral("accountName"));
}

void MessageDelegateHelperImageTest::shouldExtractMessageData()
{
    MessageDelegateHelperImage helper;
    QStyleOptionViewItem option;
    QWidget fakeWidget;
    option.widget = &fakeWidget;
    Message message;
    const MessageAttachment msgAttach = testAttachment();
    message.setAttachements({msgAttach});

    const MessageDelegateHelperImage::ImageLayout layout = helper.layoutImage(&message, option, 500, 500);
    QCOMPARE(layout.title, msgAttach.title());
    QCOMPARE(layout.description, msgAttach.description());
    QVERIFY(layout.isShown);
    QVERIFY(!layout.isAnimatedImage);
    QVERIFY(layout.imagePath.endsWith(QLatin1String("/testfile.png")));
    QCOMPARE(layout.imageSize.height(), layout.imageSize.width() * 2); // aspect ratio is preserved
    QVERIFY(layout.imageSize.height() < 500);
}
