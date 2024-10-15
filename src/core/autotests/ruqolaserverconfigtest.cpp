/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaserverconfigtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_autotest_helper.h"
#include "ruqolaserverconfig.h"

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
    QCOMPARE(config.blockEditingMessageInMinutes(), 5);
    QCOMPARE(config.blockDeletingMessageInMinutes(), 5);
    QCOMPARE(config.ruqolaOauthTypes(), AuthenticationManager::AuthMethodType::Unknown);
    QCOMPARE(config.serverAuthMethodTypes(), AuthenticationManager::AuthMethodType::Unknown);
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
    QCOMPARE(config.directMessageMaximumUser(), -1);
    QCOMPARE(config.messageQuoteChainLimit(), -1);
    QVERIFY(!config.allowCustomStatusMessage());

    QVERIFY(config.mediaBlackList().isEmpty());
    QVERIFY(config.mediaWhiteList().isEmpty());

    RuqolaServerConfig::ConfigWithDefaultValue defaultValue;
    QVERIFY(defaultValue.url.isEmpty());
    QVERIFY(defaultValue.defaultUrl.isEmpty());
    QVERIFY(defaultValue.isEmpty());

    defaultValue.url = QStringLiteral("dd");
    QVERIFY(defaultValue.isEmpty());

    defaultValue.defaultUrl = QStringLiteral("dd1");
    QVERIFY(!defaultValue.isEmpty());

    QVERIFY(!config.previewEmbed());
    QCOMPARE(config.embedCacheExpirationDays(), 30);
    QVERIFY(config.accountsDefaultUserPreferencesPushNotifications().isEmpty());
    QVERIFY(config.accountsDefaultUserPreferencesDesktopNotifications().isEmpty());
    QVERIFY(config.allowEmailNotifications());
    QVERIFY(!config.allowEmailVerification());
    QVERIFY(!config.federationEnabled());

    RuqolaServerConfig::PasswordSettings passwordSetting;
    QVERIFY(!passwordSetting.accountsPasswordPolicyEnabled);
    QVERIFY(passwordSetting.accountsPasswordPolicyForbidRepeatingCharacters);
    QVERIFY(passwordSetting.accountsPasswordPolicyAtLeastOneLowercase);
    QVERIFY(passwordSetting.accountsPasswordPolicyAtLeastOneUppercase);
    QVERIFY(passwordSetting.accountsPasswordPolicyAtLeastOneNumber);
    QVERIFY(passwordSetting.accountsPasswordPolicyAtLeastOneSpecialCharacter);
    QCOMPARE(passwordSetting.accountsPasswordPolicyMinLength, 12);
    QCOMPARE(passwordSetting.accountsPasswordPolicyMaxLength, 24);
    QCOMPARE(passwordSetting.accountsPasswordPolicyForbidRepeatingCharactersCount, 3);
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

    RuqolaServerConfig::ConfigWithDefaultValue logoUrl;
    logoUrl.url = QStringLiteral("path/to/logo");
    RuqolaServerConfig::ConfigWithDefaultValue faviconUrl;
    faviconUrl.url = QStringLiteral("path/to/favicon");

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

void RuqolaServerConfigTest::shouldVerifyOauthType_data()
{
    QTest::addColumn<QStringList>("oauthlist");
    QTest::addColumn<AuthenticationManager::AuthMethodTypes>("types");

    {
        QStringList lst;
        // By default ruqola has password support
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Unknown;
        QTest::newRow("empty") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_FaceBook")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::FaceBook;
        QTest::newRow("fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Twitter")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Twitter;
        QTest::newRow("tw") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Google;
        QTest::newRow("go") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Google"), QStringLiteral("Accounts_OAuth_Twitter"), QStringLiteral("Accounts_OAuth_FaceBook")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Google;
        types |= AuthenticationManager::AuthMethodType::FaceBook;
        types |= AuthenticationManager::AuthMethodType::Twitter;
        QTest::newRow("go-tw-fb") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Unknown;
        QTest::newRow("unknown") << lst << types;
    }
    {
        const QStringList lst = {QStringLiteral("Accounts_OAuth_Blable"), QStringLiteral("Accounts_OAuth_Twitter")};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Twitter;
        QTest::newRow("unknown-2") << lst << types;
    }
}

void RuqolaServerConfigTest::shouldVerifyOauthType()
{
    QFETCH(QStringList, oauthlist);
    QFETCH(AuthenticationManager::AuthMethodTypes, types);
    RuqolaServerConfig config;
    for (const QString &t : oauthlist) {
        config.addOauthService(t);
    }
    QCOMPARE(config.serverAuthMethodTypes(), types);
}

void RuqolaServerConfigTest::shouldVerifyThatServerSupportService()
{
    RuqolaServerConfig config;
    QVERIFY(!config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Twitter));
    config.addOauthService(QStringLiteral("Accounts_OAuth_Twitter"));
    QVERIFY(config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Twitter));

    QVERIFY(!config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::FaceBook));
    config.addOauthService(QStringLiteral("Accounts_OAuth_Google"));
    QVERIFY(config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Google));

    config.addOauthService(QStringLiteral("Accounts_OAuth_FaceBook"));
    QVERIFY(config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::FaceBook));
}

void RuqolaServerConfigTest::shouldAddRuqolaAuthenticationSupport()
{
    RuqolaServerConfig config;
    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Twitter));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::Twitter);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Twitter));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::FaceBook));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::FaceBook);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::FaceBook));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Google));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::Google);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Google));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::GitHub));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::GitHub);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::GitHub));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::GitLab));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::GitLab);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::GitLab));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Linkedin));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::Linkedin);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Linkedin));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Apple));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::Apple);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Apple));

    QVERIFY(!config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::NextCloud));
    config.addRuqolaAuthenticationSupport(AuthenticationManager::AuthMethodType::NextCloud);
    QVERIFY(config.ruqolaHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::NextCloud));
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

void RuqolaServerConfigTest::shouldSerializeConfig_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AuthenticationManager::AuthMethodTypes>("authentications");

    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::Unknown;
        QTest::newRow("empty") << QStringLiteral("empty.json") << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::Password;
        QTest::newRow("test1") << QStringLiteral("test1.json") << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        QTest::newRow("onlygithub") << QStringLiteral("onlygithub.json") << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        type |= AuthenticationManager::AuthMethodType::GitLab;
        QTest::newRow("github-gitlab") << QStringLiteral("github-gitlab.json") << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        type |= AuthenticationManager::AuthMethodType::GitLab;
        type |= AuthenticationManager::AuthMethodType::Password;
        QTest::newRow("github-gitlab-password") << QStringLiteral("github-gitlab-password.json") << type;
    }
}

void RuqolaServerConfigTest::shouldSerializeConfig()
{
    QFETCH(QString, name);
    QFETCH(AuthenticationManager::AuthMethodTypes, authentications);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/serverconfig/%1"_L1.arg(name);
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RuqolaServerConfig config;
    config.parsePublicSettings(obj, false);

    const QByteArray ba = config.serialize(false);

    const QJsonDocument doc = QJsonDocument::fromJson(ba);
    const QJsonObject newObj = doc.object();

    RuqolaServerConfig newConfig;
    newConfig.deserialize(newObj);

    bool compare = (config == newConfig);
    if (!compare) {
        qDebug() << "config " << config;
        qDebug() << "newConfig " << newConfig;
    }
    QCOMPARE(config.serverAuthMethodTypes(), authentications);
    QVERIFY(compare);
}

void RuqolaServerConfigTest::shouldCheckPassword_data()
{
    QTest::addColumn<QString>("password");
    QTest::addColumn<RuqolaServerConfig::PasswordSettings>("passwordSettings");
    QTest::addColumn<RuqolaServerConfig::PasswordSettings::PasswordSettingChecks>("checks");
    QTest::addColumn<bool>("valid");
    QTest::addColumn<RuqolaServerConfig::PasswordSettings::PasswordSettingChecks>("passwordOk");

    {
        RuqolaServerConfig::PasswordSettings settings;
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = RuqolaServerConfig::PasswordSettings::None;
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("empty") << QString() << settings << check << true << passwordOk;
    }

    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = false;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = RuqolaServerConfig::PasswordSettings::None;
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("disable") << QStringLiteral("sdfsdfDdd2") << settings << check << true << passwordOk;
    }

    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test1") << QStringLiteral("A") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
                                                                             RuqolaServerConfig::PasswordSettings::MaxLengh};
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test2") << QStringLiteral("AAAAA") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = false;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test3") << QStringLiteral("Aqsdfsdfsdf") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test4") << QStringLiteral("Aqsdfsdfsdf") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test5") << QStringLiteral("Aq1444") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test6") << QStringLiteral("Aq1444") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
                                                                             RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
                                                                             RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
                                                                             RuqolaServerConfig::PasswordSettings::MinLengh,
                                                                             RuqolaServerConfig::PasswordSettings::MaxLengh};
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test7") << QStringLiteral("Aq144444") << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test7 (less than max)") << QStringLiteral("Aq1444") << settings << check << false << passwordOk;
    }

    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test8") << QStringLiteral("Aq1444@") << settings << check << true << passwordOk;
    }

    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test8-1") << QStringLiteral("Aq1444;;") << settings << check << true << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test8-2") << QStringLiteral("Aq1444_") << settings << check << true << passwordOk;
    }

    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyAtLeastOneLowercase = true;
        settings.accountsPasswordPolicyAtLeastOneNumber = true;
        settings.accountsPasswordPolicyMinLength = 6;
        settings.accountsPasswordPolicyMaxLength = 8;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 4;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
        };
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = RuqolaServerConfig::PasswordSettings::None;
        QTest::newRow("test9") << QStringLiteral("Aq1444_") << settings << check << true << passwordOk;
    }
}

void RuqolaServerConfigTest::shouldCheckPassword()
{
    QFETCH(QString, password);
    QFETCH(RuqolaServerConfig::PasswordSettings, passwordSettings);
    QFETCH(RuqolaServerConfig::PasswordSettings::PasswordSettingChecks, checks);
    QFETCH(bool, valid);
    QFETCH(RuqolaServerConfig::PasswordSettings::PasswordSettingChecks, passwordOk);
    const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks f = passwordSettings.validatePassword(password);
    // qDebug() << " f " << f;
    QCOMPARE(f, checks);
    // qDebug() << " FGGDFGSFGSDFGS " << f;
    const bool passwordValidValue = passwordSettings.isValidatePassword(f);
    qDebug() << " passwordValidValue " << passwordValidValue << " password " << password;
    QCOMPARE(passwordValidValue, valid);
    QCOMPARE(passwordSettings.passwordValidChecks(), passwordOk);
}

#include "moc_ruqolaserverconfigtest.cpp"
