/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    QVERIFY(config.siteName().isEmpty());
    QVERIFY(config.siteUrl().isEmpty());
    QVERIFY(!config.needAdaptNewSubscriptionRC60());
    QCOMPARE(config.blockEditingMessageInMinutes(), 5);
    QCOMPARE(config.blockDeletingMessageInMinutes(), 5);
    QCOMPARE(config.ruqolaOauthTypes(), AuthenticationManager::OauthType::Password);
    QCOMPARE(config.serverOauthTypes(), AuthenticationManager::OauthType::Password);
    QVERIFY(config.autoTranslateGoogleKey().isEmpty());
    QCOMPARE(config.fileMaxFileSize(), -1);
    QVERIFY(config.logoUrl().isEmpty());
    QVERIFY(config.faviconUrl().isEmpty());
    QCOMPARE(config.loginExpiration(), -1);
    QVERIFY(config.userNameValidation().isEmpty());
    QVERIFY(config.channelNameValidation().isEmpty());
    QCOMPARE(config.messageMaximumAllowedSize(), -1);
    QVERIFY(!config.messageAllowConvertLongMessagesToAttachment());
    QVERIFY(!config.useRealName());
    QVERIFY(!config.hasEnterpriseSupport());
    QVERIFY(config.accountsAllowInvisibleStatusOption());
    QVERIFY(config.userDataDownloadEnabled());
    QVERIFY(!config.deviceManagementEnableLoginEmails());
    QVERIFY(!config.deviceManagementAllowLoginEmailpreference());
    QCOMPARE(config.messageGroupingPeriod(), -1);
}

void RuqolaServerConfigTest::shouldAssignValues()
{
    const QString jitsimeetprefix = QStringLiteral("test 1");
    const QString jitsimeeturl = QStringLiteral("test 2");
    const QString uniqueId = QStringLiteral("test 3");
    const QString filestoragetype = QStringLiteral("test 4");
    const QString googleKey = QStringLiteral("Google12");
    const int minutes = 12;
    const int minutesDeletingMessage = 15;
    const QString siteName = QStringLiteral("sitename");
    const QString siteUrl = QStringLiteral("siteurl");
    const QString logoUrl = QStringLiteral("path/to/logo");
    const QString faviconUrl = QStringLiteral("path/to/favicon");

    RuqolaServerConfig config;
    config.setJitsiMeetPrefix(jitsimeetprefix);
    config.setJitsiMeetUrl(jitsimeeturl);
    config.setUniqueId(uniqueId);
    config.setFileUploadStorageType(filestoragetype);
    config.setBlockEditingMessageInMinutes(minutes);
    config.setBlockDeletingMessageInMinutes(minutesDeletingMessage);
    config.setSiteName(siteName);
    config.setSiteUrl(siteUrl);
    config.setAutoTranslateGoogleKey(googleKey);
    config.setLogoUrl(logoUrl);
    config.setFaviconUrl(faviconUrl);

    quint64 uploadFileMax = 222222;

    config.setFileMaxFileSize(uploadFileMax);

    QCOMPARE(config.jitsiMeetPrefix(), jitsimeetprefix);
    QCOMPARE(config.jitsiMeetUrl(), jitsimeeturl);
    QCOMPARE(config.uniqueId(), uniqueId);
    QCOMPARE(config.fileUploadStorageType(), filestoragetype);
    QCOMPARE(config.blockEditingMessageInMinutes(), minutes);
    QCOMPARE(config.blockDeletingMessageInMinutes(), minutesDeletingMessage);
    QCOMPARE(config.fileMaxFileSize(), uploadFileMax);

    QCOMPARE(config.siteUrl(), siteUrl);
    QCOMPARE(config.siteName(), siteName);

    QCOMPARE(config.autoTranslateGoogleKey(), googleKey);

    QCOMPARE(config.logoUrl(), logoUrl);
    QCOMPARE(config.faviconUrl(), faviconUrl);

    QCOMPARE(config.serverConfigFeatureTypes(), RuqolaServerConfig::ServerConfigFeatureType::None);
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
    QTest::newRow("1.0.0-develop") << QStringLiteral("1.0.0") << true;
    QTest::newRow("4.0") << QStringLiteral("4.0") << true;
}

void RuqolaServerConfigTest::shouldEnabledRc60()
{
    QFETCH(QString, serverVersion);
    QFETCH(bool, needRc60);
    RuqolaServerConfig config;
    config.setServerVersion(serverVersion);
    // qDebug() << " config " << config;
    QCOMPARE(config.needAdaptNewSubscriptionRC60(), needRc60);
}

void RuqolaServerConfigTest::shouldVerifyOauthType_data()
{
    QTest::addColumn<QStringList>("oauthlist");
    QTest::addColumn<AuthenticationManager::OauthTypes>("types");

    {
        QStringList lst;
        // By default ruqola has password support
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Unknown;
        QTest::newRow("empty") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_FaceBook")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::FaceBook;
        QTest::newRow("fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Twitter")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Twitter;
        QTest::newRow("tw") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Google;
        QTest::newRow("go") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google"), QStringLiteral("Accounts_OAuth_Twitter"), QStringLiteral("Accounts_OAuth_FaceBook")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Google;
        types |= AuthenticationManager::OauthType::FaceBook;
        types |= AuthenticationManager::OauthType::Twitter;
        QTest::newRow("go-tw-fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Unknown;
        QTest::newRow("unknown") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable"), QStringLiteral("Accounts_OAuth_Twitter")};
        AuthenticationManager::OauthTypes types = AuthenticationManager::OauthType::Password;
        types |= AuthenticationManager::OauthType::Twitter;
        QTest::newRow("unknow-2") << lst << types;
    }
}

void RuqolaServerConfigTest::shouldVerifyOauthType()
{
    QFETCH(QStringList, oauthlist);
    QFETCH(AuthenticationManager::OauthTypes, types);
    RuqolaServerConfig config;
    for (const QString &t : oauthlist) {
        config.addOauthService(t);
    }
    QCOMPARE(config.serverOauthTypes(), types);
}

void RuqolaServerConfigTest::shouldVerifyThatServerSupportService()
{
    RuqolaServerConfig config;
    QVERIFY(!config.serverHasSupportForOauthType(AuthenticationManager::OauthType::Twitter));
    config.addOauthService(QStringLiteral("Accounts_OAuth_Twitter"));
    QVERIFY(config.serverHasSupportForOauthType(AuthenticationManager::OauthType::Twitter));

    QVERIFY(!config.serverHasSupportForOauthType(AuthenticationManager::OauthType::FaceBook));
    config.addOauthService(QStringLiteral("Accounts_OAuth_Google"));
    QVERIFY(config.serverHasSupportForOauthType(AuthenticationManager::OauthType::Google));

    config.addOauthService(QStringLiteral("Accounts_OAuth_FaceBook"));
    QVERIFY(config.serverHasSupportForOauthType(AuthenticationManager::OauthType::FaceBook));
}

void RuqolaServerConfigTest::shouldAddRuqolaAuthenticationSupport()
{
    RuqolaServerConfig config;
    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Twitter));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::Twitter);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Twitter));

    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::FaceBook));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::FaceBook);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::FaceBook));

    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Google));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::Google);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Google));

    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::GitHub));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::GitHub);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::GitHub));

    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::GitLab));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::GitLab);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::GitLab));

    QVERIFY(!config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Linkedin));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::OauthType::Linkedin);
    QVERIFY(config.ruqolaHasSupportForOauthType(AuthenticationManager::OauthType::Linkedin));
}

void RuqolaServerConfigTest::shouldTestVersion_data()
{
    QTest::addColumn<QString>("serverVersion");
    QTest::addColumn<int>("major");
    QTest::addColumn<int>("minor");
    QTest::addColumn<int>("patch");
    QTest::addColumn<bool>("hasCorrectVersion");
    QTest::newRow("0.60.0") << QStringLiteral("0.60.0") << 0 << 60 << 0 << true;
    QTest::newRow("0.59.0-incorrect") << QStringLiteral("0.59.0") << 0 << 60 << 0 << false;
    QTest::newRow("0.60.0-supperior") << QStringLiteral("0.61.0") << 0 << 60 << 0 << true;
    QTest::newRow("0.60.0-supperior-2") << QStringLiteral("0.60.1") << 0 << 60 << 0 << true;
    QTest::newRow("1.0.0-develop") << QStringLiteral("1.0.0-develop") << 0 << 60 << 0 << true;
    QTest::newRow("4.0") << QStringLiteral("4.0") << 4 << 0 << 0 << true;
}

void RuqolaServerConfigTest::shouldTestVersion()
{
    QFETCH(QString, serverVersion);
    QFETCH(int, major);
    QFETCH(int, minor);
    QFETCH(int, patch);
    QFETCH(bool, hasCorrectVersion);

    RuqolaServerConfig config;
    config.setServerVersion(serverVersion);
    QCOMPARE(config.hasAtLeastVersion(major, minor, patch), hasCorrectVersion);
}
