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

#include "ruqolaserverconfigtest.h"
#include "ruqolaserverconfig.h"

#include <QTest>

QTEST_GUILESS_MAIN(RuqolaServerConfigTest)

RuqolaServerConfigTest::RuqolaServerConfigTest(QObject *parent)
    : QObject(parent)
{
}

void RuqolaServerConfigTest::shouldHaveDefaultValues()
{
    RuqolaServerConfig config;
    QVERIFY(config.jitsiMeetPrefix().isEmpty());
    QVERIFY(config.jitsiMeetUrl().isEmpty());
    QVERIFY(config.uniqueId().isEmpty());
    QVERIFY(config.fileUploadStorageType().isEmpty());
    QVERIFY(config.allowMessageEditing());
    QVERIFY(config.otrEnabled());
    QVERIFY(!config.needAdaptNewSubscriptionRC60());
    QCOMPARE(config.blockEditingMessageInMinutes(), 5);
    QCOMPARE(config.oauthTypes(), RuqolaServerConfig::OauthType::Unknown);
}

void RuqolaServerConfigTest::shouldAssignValues()
{
    const QString jitsimeetprefix = QStringLiteral("test 1");
    const QString jitsimeeturl = QStringLiteral("test 2");
    const QString uniqueId = QStringLiteral("test 3");
    const QString filestoragetype = QStringLiteral("test 4");
    const bool allowEditing = false;
    const int minutes = 12;
    const bool otrEnable = false;
    RuqolaServerConfig config;
    config.setJitsiMeetPrefix(jitsimeetprefix);
    config.setJitsiMeetUrl(jitsimeeturl);
    config.setUniqueId(uniqueId);
    config.setFileUploadStorageType(filestoragetype);
    config.setAllowMessageEditing(allowEditing);
    config.setBlockEditingMessageInMinutes(minutes);
    config.setOtrEnabled(otrEnable);

    QCOMPARE(config.jitsiMeetPrefix(), jitsimeetprefix);
    QCOMPARE(config.jitsiMeetUrl(), jitsimeeturl);
    QCOMPARE(config.uniqueId(), uniqueId);
    QCOMPARE(config.fileUploadStorageType(), filestoragetype);
    QCOMPARE(config.allowMessageEditing(), allowEditing);
    QCOMPARE(config.blockEditingMessageInMinutes(), minutes);
    QCOMPARE(config.otrEnabled(), otrEnable);
}

void RuqolaServerConfigTest::shouldEnabledRc60_data()
{
    QTest::addColumn<QString>("serverVersion");
    QTest::addColumn<bool>("needRc60");
    QTest::newRow("0.1.0") << QStringLiteral("0.1.0") << false;
    QTest::newRow("0.60.0") << QStringLiteral("0.60.0") << true;
    QTest::newRow("0.70.0") << QStringLiteral("0.70.0") << true;
    QTest::newRow("invalid") << QStringLiteral("foo") << false;
    QTest::newRow("invalid-2") << QStringLiteral("0.6foo") << false;
}

void RuqolaServerConfigTest::shouldEnabledRc60()
{
    QFETCH(QString, serverVersion);
    QFETCH(bool, needRc60);
    RuqolaServerConfig config;
    config.setServerVersion(serverVersion);
    QCOMPARE(config.needAdaptNewSubscriptionRC60(), needRc60);
}

void RuqolaServerConfigTest::shouldVerifyOauthType_data()
{
    QTest::addColumn<QStringList>("oauthlist");
    QTest::addColumn<RuqolaServerConfig::OauthTypes>("types");

    {
        QStringList lst;
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Unknown;
        QTest::newRow("empty") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_FaceBook")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::FaceBook;
        QTest::newRow("fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Twitter")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Twitter;
        QTest::newRow("tw") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Google;
        QTest::newRow("go") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google"), QStringLiteral("Accounts_OAuth_Twitter"), QStringLiteral("Accounts_OAuth_FaceBook")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Google;
        types |= RuqolaServerConfig::OauthType::FaceBook;
        types |= RuqolaServerConfig::OauthType::Twitter;
        QTest::newRow("go-tw-fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Unknown;
        QTest::newRow("unknow") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable"), QStringLiteral("Accounts_OAuth_Twitter")};
        RuqolaServerConfig::OauthTypes types;
        types |= RuqolaServerConfig::OauthType::Twitter;
        QTest::newRow("unknow-2") << lst << types;
    }

}

void RuqolaServerConfigTest::shouldVerifyOauthType()
{
    QFETCH(QStringList, oauthlist);
    QFETCH(RuqolaServerConfig::OauthTypes, types);
    RuqolaServerConfig config;
    for (const QString &t : oauthlist) {
        config.addOauthService(t);
    }
    QCOMPARE(config.oauthTypes(), types);
}
