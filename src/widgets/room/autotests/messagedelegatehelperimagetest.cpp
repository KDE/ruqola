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
#include <QTest>

QTEST_MAIN(MessageDelegateHelperImageTest)

MessageDelegateHelperImageTest::MessageDelegateHelperImageTest(QObject *parent)
    : QObject(parent)
{
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

