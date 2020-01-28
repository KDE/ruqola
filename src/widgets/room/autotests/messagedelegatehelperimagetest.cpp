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
#include "room/messagedelegatehelperimage.h"
#include "managerdatapaths.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messages/message.h"
#include "messages/messageattachment.h"

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
    MessageAttachment msgAttach;
    const QString title = QStringLiteral("This is the title");
    msgAttach.setTitle(title);
    const QString description = QStringLiteral("A description");
    msgAttach.setDescription(description);
    QPixmap pix(50, 50);
    pix.fill(Qt::white);
    // Save the pixmap directly into the cache so that no download hpapens
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, Ruqola::self()->rocketChatAccount()->accountName());
    const QString link = QLatin1String("/testfile.png");
    const QString pixFileName = cachePath + link;
    pix.save(pixFileName, "png");
    msgAttach.setLink(link);
    message.setAttachements({msgAttach});
    const MessageDelegateHelperImage::ImageLayout layout = helper.layoutImage(&message, option);
    QCOMPARE(layout.title, title);
    QCOMPARE(layout.description, description);
    QVERIFY(layout.isShown);
}

void MessageDelegateHelperImageTest::shouldCacheLastFivePixmaps()
{
    MessageDelegateHelperImage helper;
    for (int i = 0; i < 6; ++i) {
        const QString link = QStringLiteral("link") + QString::number(i);
        const QPixmap pix(5, 5);
        helper.insertCachedPixmap(link, pix);
        QVERIFY(!helper.findCachedPixmap(link).isNull());
        QVERIFY(!helper.findCachedPixmap(QStringLiteral("link0")).isNull()); // keeps being used
    }
    QVERIFY(!helper.findCachedPixmap(QStringLiteral("link3")).isNull());
    QVERIFY(helper.findCachedPixmap(QStringLiteral("link1")).isNull()); // oldest one got evicted
}
