/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaserverconfigtest.h"

#include "ruqola_autotest_helper.h"
#include "ruqolaserverconfig.h"

QTEST_GUILESS_MAIN(RuqolaServerConfigTest)

using namespace Qt::Literals::StringLiterals;
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

    defaultValue.url = u"dd"_s;
    QVERIFY(defaultValue.isEmpty());

    defaultValue.defaultUrl = u"dd1"_s;
    QVERIFY(!defaultValue.isEmpty());

    QVERIFY(!config.previewEmbed());
    QCOMPARE(config.embedCacheExpirationDays(), 30);
    QVERIFY(config.accountsDefaultUserPreferencesPushNotifications().isEmpty());
    QVERIFY(config.accountsDefaultUserPreferencesDesktopNotifications().isEmpty());
    QVERIFY(config.allowEmailNotifications());
    QVERIFY(!config.allowEmailVerification());
    QVERIFY(!config.federationEnabled());
    QVERIFY(!config.accountsManuallyApproveNewUsers());

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
    const QString jitsimeetprefix = u"test 1"_s;
    const QString jitsimeeturl = u"test 2"_s;
    const QString uniqueId = u"test 3"_s;
    const QString filestoragetype = u"test 4"_s;
    const QString googleKey = u"Google12"_s;
    const int minutes = 12;
    const int minutesDeletingMessage = 15;
    const QString siteName = u"sitename"_s;
    const QString siteUrl = u"siteurl"_s;

    RuqolaServerConfig::ConfigWithDefaultValue logoUrl;
    logoUrl.url = u"path/to/logo"_s;
    RuqolaServerConfig::ConfigWithDefaultValue faviconUrl;
    faviconUrl.url = u"path/to/favicon"_s;

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
        const QStringList lst = {u"Accounts_OAuth_FaceBook"_s};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::FaceBook;
        QTest::newRow("fb") << lst << types;
    }
    {
        const QStringList lst = {u"Accounts_OAuth_Twitter"_s};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Twitter;
        QTest::newRow("tw") << lst << types;
    }
    {
        const QStringList lst = {u"Accounts_OAuth_Google"_s};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Google;
        QTest::newRow("go") << lst << types;
    }
    {
        const QStringList lst = {u"Accounts_OAuth_Google"_s, u"Accounts_OAuth_Twitter"_s, u"Accounts_OAuth_FaceBook"_s};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Google;
        types |= AuthenticationManager::AuthMethodType::FaceBook;
        types |= AuthenticationManager::AuthMethodType::Twitter;
        QTest::newRow("go-tw-fb") << lst << types;
    }
    {
        const QStringList lst = {u"Accounts_OAuth_Blable"_s};
        AuthenticationManager::AuthMethodTypes types;
        types |= AuthenticationManager::AuthMethodType::Unknown;
        QTest::newRow("unknown") << lst << types;
    }
    {
        const QStringList lst = {u"Accounts_OAuth_Blable"_s, u"Accounts_OAuth_Twitter"_s};
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
    config.addOauthService(u"Accounts_OAuth_Twitter"_s);
    QVERIFY(config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Twitter));

    QVERIFY(!config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::FaceBook));
    config.addOauthService(u"Accounts_OAuth_Google"_s);
    QVERIFY(config.serverHasSupportForAuthMethodType(AuthenticationManager::AuthMethodType::Google));

    config.addOauthService(u"Accounts_OAuth_FaceBook"_s);
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
    QTest::newRow("0.60.0") << u"0.60.0"_s << 0 << 60 << 0 << true;
    QTest::newRow("0.59.0-incorrect") << u"0.59.0"_s << 0 << 60 << 0 << false;
    QTest::newRow("0.60.0-supperior") << u"0.61.0"_s << 0 << 60 << 0 << true;
    QTest::newRow("0.60.0-supperior-2") << u"0.60.1"_s << 0 << 60 << 0 << true;
    QTest::newRow("1.0.0-develop") << u"1.0.0-develop"_s << 0 << 60 << 0 << true;
    QTest::newRow("4.0") << u"4.0"_s << 4 << 0 << 0 << true;
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
        QTest::newRow("empty") << u"empty.json"_s << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::Password;
        QTest::newRow("test1") << u"test1.json"_s << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        QTest::newRow("onlygithub") << u"onlygithub.json"_s << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        type |= AuthenticationManager::AuthMethodType::GitLab;
        QTest::newRow("github-gitlab") << u"github-gitlab.json"_s << type;
    }
    {
        AuthenticationManager::AuthMethodTypes type = AuthenticationManager::AuthMethodType::GitHub;
        type |= AuthenticationManager::AuthMethodType::GitLab;
        type |= AuthenticationManager::AuthMethodType::Password;
        QTest::newRow("github-gitlab-password") << u"github-gitlab-password.json"_s << type;
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
        QTest::newRow("disable") << u"sdfsdfDdd2"_s << settings << check << true << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };

        QTest::newRow("test1") << u"A"_s << settings << check << false << passwordOk;
    }
    {
        RuqolaServerConfig::PasswordSettings settings;
        settings.accountsPasswordPolicyEnabled = true;
        settings.accountsPasswordPolicyAtLeastOneUppercase = true;
        settings.accountsPasswordPolicyForbidRepeatingCharactersCount = 3;
        RuqolaServerConfig::PasswordSettings::PasswordSettingChecks check = {RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
                                                                             RuqolaServerConfig::PasswordSettings::MaxLengh};
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test2") << u"AAAAA"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test3") << u"Aqsdfsdfsdf"_s << settings << check << false << passwordOk;
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

        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test4") << u"Aqsdfsdfsdf"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test5") << u"Aq1444"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test6") << u"Aq1444"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };

        QTest::newRow("test7") << u"Aq144444"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test7 (less than max)") << u"Aq1444"_s << settings << check << false << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test8") << u"Aq1444@"_s << settings << check << true << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test8-1") << u"Aq1444;;"_s << settings << check << true << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test8-2") << u"Aq1444_"_s << settings << check << true << passwordOk;
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
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks passwordOk = {
            RuqolaServerConfig::PasswordSettings::AtLeastOneUppercase,
            RuqolaServerConfig::PasswordSettings::MaxLengh,
            RuqolaServerConfig::PasswordSettings::AtLeastOneLowercase,
            RuqolaServerConfig::PasswordSettings::AtLeastOneNumber,
            RuqolaServerConfig::PasswordSettings::MinLengh,
            RuqolaServerConfig::PasswordSettings::ForbidRepeatingCharactersCount,
            RuqolaServerConfig::PasswordSettings::AtLeastOneSpecialCharacter,
        };
        QTest::newRow("test9") << u"Aq1444_"_s << settings << check << true << passwordOk;
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
    // qDebug() << " passwordValidValue " << passwordValidValue << " password " << password;
    QCOMPARE(passwordValidValue, valid);
    QCOMPARE(passwordSettings.passwordValidChecks(), passwordOk);
}

#include "moc_ruqolaserverconfigtest.cpp"
