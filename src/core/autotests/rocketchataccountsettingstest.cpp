/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountsettingstest.h"
#include "rocketchataccountsettings.h"
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(RocketChatAccountSettingsTest)

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

    // Test same url
    SampleChatAccount.setServerUrl(serverUrlDefault);
    QCOMPARE(SpyURL.count(), 1);

    // Test Empty url
    const QString emptyString;
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

    // Test same name
    SampleChat.setUserName(userNameDefault);
    QCOMPARE(SpyName.count(), 1);

    // Test empty string
    const QString emptyString;
    SampleChat.setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);

    SampleChat.setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenUserIDChanged()
{
    RocketChatAccountSettings SampleChat1;

    QSignalSpy SpyID(&SampleChat1, &RocketChatAccountSettings::userIdChanged);
    const QString userId = QStringLiteral("RA15");
    QVERIFY(userId != SampleChat1.userId());
    SampleChat1.setUserId(QStringLiteral("RA15"));
    QCOMPARE(SpyID.count(), 1);
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenLoginStatusChanged()
{
    // TODO
}

void RocketChatAccountSettingsTest::shouldLogout()
{
    RocketChatAccountSettings SampleChat;

    SampleChat.setAuthToken(QStringLiteral("Token305"));
    SampleChat.setUserId(QStringLiteral("ECE305"));
    SampleChat.setPassword(QStringLiteral("masterPassword"));
    // Make sure that values are not null
    QVERIFY(!SampleChat.authToken().isEmpty());
    QVERIFY(!SampleChat.userId().isEmpty());
    QVERIFY(!SampleChat.password().isEmpty());

    SampleChat.logout();
    QVERIFY(SampleChat.authToken().isEmpty());
    QVERIFY(SampleChat.userId().isEmpty());
    QVERIFY(!SampleChat.password().isEmpty()); // logout should allow to log back in easily, don't clear the password
}

void RocketChatAccountSettingsTest::shouldSetAccountName()
{
    RocketChatAccountSettings sampleChat;
    QSignalSpy spy(&sampleChat, &RocketChatAccountSettings::accountNameChanged);

    const QString val = QStringLiteral("myAccount#$^56");
    sampleChat.setAccountName(val);

    QCOMPARE(val, sampleChat.accountName());
    QCOMPARE(spy.count(), 1);
}

void RocketChatAccountSettingsTest::shouldsetAuthToken()
{
    RocketChatAccountSettings sampleChat;

    const QString val = QStringLiteral("myAuthToken#$^56");
    sampleChat.setAuthToken(val);

    QCOMPARE(val, sampleChat.authToken());
}

void RocketChatAccountSettingsTest::shouldSetPassword()
{
    RocketChatAccountSettings sampleChat;

    const QString val = QStringLiteral("myPass#$^56");
    sampleChat.setPassword(val);

    QCOMPARE(val, sampleChat.password());
}

void RocketChatAccountSettingsTest::shouldSetServerUrl()
{
    RocketChatAccountSettings sampleChat;

    const QString val = QStringLiteral("my.fancy.url");
    sampleChat.setServerUrl(val);

    QCOMPARE(val, sampleChat.serverUrl());
}

void RocketChatAccountSettingsTest::shouldSetUserID()
{
    RocketChatAccountSettings sampleChat;

    const QString val = QStringLiteral("ECE305");
    sampleChat.setUserId(val);

    QCOMPARE(val, sampleChat.userId());
}

void RocketChatAccountSettingsTest::shouldSetUserName()
{
    RocketChatAccountSettings sampleChat;

    const QString val = QStringLiteral("Eric Roberts");
    sampleChat.setUserName(val);

    QCOMPARE(val, sampleChat.userName());
}

void RocketChatAccountSettingsTest::shouldHaveDefaultValues()
{
    RocketChatAccountSettings chat;

    QVERIFY(chat.accountName().isEmpty());
    QVERIFY(chat.authToken().isEmpty());
    QVERIFY(!chat.cacheBasePath().isEmpty());
    QVERIFY(chat.userId().isEmpty());
    QVERIFY(chat.userName().isEmpty());
    QVERIFY(chat.password().isEmpty());
    QVERIFY(!chat.serverUrl().isEmpty());
    QVERIFY(chat.userId().isEmpty());
    QVERIFY(chat.userName().isEmpty());
    QVERIFY(chat.authToken().isEmpty());
    QCOMPARE(chat.serverUrl(), QStringLiteral("open.rocket.chat"));
    QVERIFY(chat.twoFactorAuthenticationCode().isEmpty());
    QVERIFY(chat.accountEnabled());
    QVERIFY(chat.displayName().isEmpty());
}

#include "moc_rocketchataccountsettingstest.cpp"
