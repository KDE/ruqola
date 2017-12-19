/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

QTEST_MAIN(RocketChatAccountSettingsTest)

RocketChatAccountSettingsTest::RocketChatAccountSettingsTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatAccountSettingsTest::shouldNotEmitSignalWhenNewServerUrlIsSameAsOld()
{
    RocketChatAccountSettings *SampleChatAccount = new RocketChatAccountSettings;

    QString url = QStringLiteral("some.url.com");
    SampleChatAccount->setServerUrl(url);

    QSignalSpy SpyURL(SampleChatAccount, &RocketChatAccountSettings::serverURLChanged);
    SampleChatAccount->setServerUrl(url);
    QCOMPARE(SpyURL.count(), 0);

    delete SampleChatAccount;
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenSetServerURLChanged()
{
    RocketChatAccountSettings *SampleChatAccount = new RocketChatAccountSettings;

    QSignalSpy SpyURL(SampleChatAccount, &RocketChatAccountSettings::serverURLChanged);
    SampleChatAccount->setServerUrl(QStringLiteral("bla bla"));
    QCOMPARE(SpyURL.count(), 1);

    QString emptyString;
    SampleChatAccount->setServerUrl(emptyString);
    QCOMPARE(SpyURL.count(), 2);

    SampleChatAccount->setServerUrl(emptyString);
    QCOMPARE(SpyURL.count(), 2);

    delete SampleChatAccount;
}

void RocketChatAccountSettingsTest::shouldNotEmitSignalWhenNewUsernameIsSameAsOld()
{
    RocketChatAccountSettings *SampleChat = new RocketChatAccountSettings;

    QString username = QStringLiteral("dummyUsername");
    SampleChat->setUserName(username);

    QSignalSpy SpyName(SampleChat, &RocketChatAccountSettings::userNameChanged);
    SampleChat->setUserName(username);
    QCOMPARE(SpyName.count(), 0);

    delete SampleChat;
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenUserNameChanged()
{
    RocketChatAccountSettings *SampleChat = new RocketChatAccountSettings;

    QSignalSpy SpyName(SampleChat, &RocketChatAccountSettings::userNameChanged);
    SampleChat->setUserName(QStringLiteral("Donald Knuth"));
    QCOMPARE(SpyName.count(), 1);

    QString emptyString;
    SampleChat->setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);

    SampleChat->setUserName(emptyString);
    QCOMPARE(SpyName.count(), 2);

    delete SampleChat;
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenUserIDChanged()
{
    RocketChatAccountSettings *SampleChat1 = new RocketChatAccountSettings;

    QSignalSpy SpyID(SampleChat1, &RocketChatAccountSettings::userIDChanged);
    SampleChat1->setUserId(QStringLiteral("RA15"));
    QCOMPARE(SpyID.count(), 1);

    delete SampleChat1;
}

void RocketChatAccountSettingsTest::shouldEmitSignalWhenLoginStatusChanged()
{

}

void RocketChatAccountSettingsTest::shouldLogout()
{
    RocketChatAccountSettings *SampleChat = new RocketChatAccountSettings;

    SampleChat->setAuthToken(QStringLiteral("Token305"));
    SampleChat->setUserId(QStringLiteral("ECE305"));
    SampleChat->setPassword(QStringLiteral("masterPassword"));

    SampleChat->logout();
    QVERIFY(SampleChat->authToken().isEmpty());
    QVERIFY(SampleChat->userId().isEmpty());
    QVERIFY(SampleChat->password().isEmpty());
}

void RocketChatAccountSettingsTest::shouldSetAccountName()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("myAccount#$^56");
    sampleChat->setAccountName(val);

    QCOMPARE(val, sampleChat->accountName());

    delete sampleChat;
}

void RocketChatAccountSettingsTest::shouldsetAuthToken()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("myAuthToken#$^56");
    sampleChat->setAuthToken(val);

    QCOMPARE(val, sampleChat->authToken());

    delete sampleChat;
}

void RocketChatAccountSettingsTest::shouldSetPassword()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("myPass#$^56");
    sampleChat->setPassword(val);

    QCOMPARE(val, sampleChat->password());

    delete sampleChat;
}

void RocketChatAccountSettingsTest::shouldSetServerUrl()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("my.fancy.url");
    sampleChat->setServerUrl(val);

    QCOMPARE(val, sampleChat->serverUrl());

    delete sampleChat;
}

void RocketChatAccountSettingsTest::shouldSetUserID()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("ECE305");
    sampleChat->setUserId(val);

    QCOMPARE(val, sampleChat->userId());

    delete sampleChat;
}

void RocketChatAccountSettingsTest::shouldSetUserName()
{
    RocketChatAccountSettings *sampleChat = new RocketChatAccountSettings;

    QString val = QStringLiteral("Eric Roberts");
    sampleChat->setUserName(val);

    QCOMPARE(val, sampleChat->userName());

    delete sampleChat;
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
