/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "rocketchataccountsettingstest.h"
#include "rocketchataccountsettings.h"
#include <QTest>
#include <QSignalSpy>
#include <QStandardPaths>

QTEST_MAIN(RocketChatAccountSettingsTest)

RocketChatAccountSettingsTest::RocketChatAccountSettingsTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatAccountSettingsTest::shouldNotEmitSignalWhenNewServerUrlIsSameAsOld()
{
    RocketChatAccountSettings SampleChatAccount;

    const QString url = QStringLiteral("some.url.com");
    SampleChatAccount.setServerUrl(url);

    QSignalSpy SpyURL(&SampleChatAccount, &RocketChatAccountSettings::serverURLChanged);
    SampleChatAccount.setServerUrl(url);
    QCOMPARE(SpyURL.count(), 0);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenSetServerURLChanged()
{
    RocketChatAccountSettings SampleChatAccount;

    QSignalSpy SpyURL(&SampleChatAccount, &RocketChatAccountSettings::serverURLChanged);
    const QString serverUrlDefault = QStringLiteral("bla bla");
    SampleChatAccount.setServerUrl(serverUrlDefault);
    QCOMPARE(SpyURL.count(), 1);

    //Test same url
    SampleChatAccount.setServerUrl(serverUrlDefault);
    QCOMPARE(SpyURL.count(), 1);

    //Test Empty url
    QString emptyString;
    SampleChatAccount.setServerUrl(emptyString);
    QCOMPARE(SpyURL.count(), 2);

    SampleChatAccount.setServerUrl(emptyString);
    QCOMPARE(SpyURL.count(), 2);
}

void RocketChatAccountSettingsTest::shouldNotEmitSignalWhenNewUsernameIsSameAsOld()
{
    RocketChatAccountSettings SampleChat;

    const QString username = QStringLiteral("dummyUsername");
    SampleChat.setUserName(username);

    QSignalSpy SpyName(&SampleChat, &RocketChatAccountSettings::userNameChanged);
    SampleChat.setUserName(username);
    QCOMPARE(SpyName.count(), 0);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenUserNameChanged()
{
    RocketChatAccountSettings SampleChat;

    QSignalSpy SpyName(&SampleChat, &RocketChatAccountSettings::userNameChanged);
    const QString userNameDefault = QStringLiteral("Donald Knuth");
    SampleChat.setUserName(userNameDefault);
    QCOMPARE(SpyName.count(), 1);

    //Test same name
    SampleChat.setUserName(userNameDefault);
    QCOMPARE(SpyName.count(), 1);

    //Test empty string
    QString emptyString;
    SampleChat.setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);

    SampleChat.setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenUserIDChanged()
{
    RocketChatAccountSettings SampleChat1;

    QSignalSpy SpyID(&SampleChat1, &RocketChatAccountSettings::userIDChanged);
    const QString userId = QStringLiteral("RA15");
    QVERIFY(userId != SampleChat1.userId());
    SampleChat1.setUserId(QStringLiteral("RA15"));
    QCOMPARE(SpyID.count(), 1);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenLoginStatusChanged()
{
}

void RocketChatAccountSettingsTest::shouldLogout()
{
    RocketChatAccountSettings SampleChat;

    SampleChat.setAuthToken(QStringLiteral("Token305"));
    SampleChat.setUserId(QStringLiteral("ECE305"));
    SampleChat.setPassword(QStringLiteral("masterPassword"));
    //Make sure that values are not null
    QVERIFY(!SampleChat.authToken().isEmpty());
    QVERIFY(!SampleChat.userId().isEmpty());
    QVERIFY(!SampleChat.password().isEmpty());

    SampleChat.logout();
    QVERIFY(SampleChat.authToken().isEmpty());
    QVERIFY(SampleChat.userId().isEmpty());
    QVERIFY(SampleChat.password().isEmpty());
}

void RocketChatAccountSettingsTest::shouldSetAccountName()
{
    RocketChatAccountSettings sampleChat;
    QSignalSpy spy(&sampleChat, &RocketChatAccountSettings::accountNameChanged);

    QString val = QStringLiteral("myAccount#$^56");
    sampleChat.setAccountName(val);

    QCOMPARE(val, sampleChat.accountName());
    QCOMPARE(spy.count(), 1);
}

void RocketChatAccountSettingsTest::shouldsetAuthToken()
{
    RocketChatAccountSettings sampleChat;

    QString val = QStringLiteral("myAuthToken#$^56");
    sampleChat.setAuthToken(val);

    QCOMPARE(val, sampleChat.authToken());
}

void RocketChatAccountSettingsTest::shouldSetPassword()
{
    RocketChatAccountSettings sampleChat;

    QString val = QStringLiteral("myPass#$^56");
    sampleChat.setPassword(val);

    QCOMPARE(val, sampleChat.password());
}

void RocketChatAccountSettingsTest::shouldSetServerUrl()
{
    RocketChatAccountSettings sampleChat;

    QString val = QStringLiteral("my.fancy.url");
    sampleChat.setServerUrl(val);

    QCOMPARE(val, sampleChat.serverUrl());
}

void RocketChatAccountSettingsTest::shouldSetUserID()
{
    RocketChatAccountSettings sampleChat;

    QString val = QStringLiteral("ECE305");
    sampleChat.setUserId(val);

    QCOMPARE(val, sampleChat.userId());
}

void RocketChatAccountSettingsTest::shouldSetUserName()
{
    RocketChatAccountSettings sampleChat;

    QString val = QStringLiteral("Eric Roberts");
    sampleChat.setUserName(val);

    QCOMPARE(val, sampleChat.userName());
}

void RocketChatAccountSettingsTest::shouldHaveDefaultValues()
{
    RocketChatAccountSettings chat;

    QVERIFY(chat.accountName().isEmpty());
    QVERIFY(chat.authToken().isEmpty());
    QVERIFY(chat.cacheBasePath().isEmpty());
    QVERIFY(chat.userId().isEmpty());
    QVERIFY(chat.userName().isEmpty());
    QVERIFY(chat.password().isEmpty());
    QVERIFY(chat.serverUrl().isEmpty());

    chat.loadSettings();
    QVERIFY(chat.userId().isEmpty());
    QVERIFY(chat.userName().isEmpty());
    QVERIFY(chat.authToken().isEmpty());
    QCOMPARE(chat.serverUrl(), QStringLiteral("demo.rocket.chat"));
}
