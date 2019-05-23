/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "rocketchataccounttest.h"
#include "rocketchataccount.h"
#include "model/searchmessagefilterproxymodel.h"
#include "model/searchmessagemodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/discussionsmodel.h"
#include "model/threadsfilterproxymodel.h"
#include "model/threadsmodel.h"
#include "model/mentionsmodel.h"
#include "model/mentionsfilterproxymodel.h"
#include <QTest>
#include <QStandardPaths>

QTEST_GUILESS_MAIN(RocketChatAccountTest)

RocketChatAccountTest::RocketChatAccountTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatAccountTest::shouldHaveDefaultValue()
{
    RocketChatAccount w;
    QVERIFY(w.emojiManager());
    QVERIFY(w.roomModel());
    QVERIFY(w.roomFilterProxyModel());
    QVERIFY(w.settings());
    QVERIFY(!w.ruqolaLogger());
    QVERIFY(w.ruqolaServerConfig());
    QVERIFY(w.userCompleterFilterModelProxy());
    QVERIFY(w.loginMethodModel());
    QVERIFY(w.inputCompleterModel());
    QVERIFY(!w.ruqolaLogger());
    QVERIFY(w.searchChannelFilterProxyModel());
    QVERIFY(w.searchMessageModel());
    QCOMPARE(w.searchMessageFilterProxyModel()->sourceModel(), w.searchMessageModel());
    QVERIFY(w.receiveTypingNotificationManager());
    QVERIFY(!w.editingMode());
    QVERIFY(w.filesModelForRoom());
    QVERIFY(w.filesForRoomFilterProxyModel());
    QCOMPARE(w.filesForRoomFilterProxyModel()->sourceModel(), w.filesModelForRoom());
    QVERIFY(w.discussionsFilterProxyModel());
    QVERIFY(w.discussionsFilterProxyModel());
    QCOMPARE(w.discussionsFilterProxyModel()->sourceModel(), w.discussionsModel());
    QVERIFY(w.threadsModel());
    QVERIFY(w.threadsFilterProxyModel());
    QCOMPARE(w.threadsFilterProxyModel()->sourceModel(), w.threadsModel());
    QVERIFY(w.mentionsFilterProxyModel());
    QCOMPARE(w.mentionsFilterProxyModel()->sourceModel(), w.mentionsModel());
}
