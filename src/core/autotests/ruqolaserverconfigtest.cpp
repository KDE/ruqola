/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
    QVERIFY(!config.jitsiEnabled());
    QVERIFY(config.uniqueId().isEmpty());
    QVERIFY(config.fileUploadStorageType().isEmpty());
    QVERIFY(config.siteName().isEmpty());
    QVERIFY(config.siteUrl().isEmpty());
    QVERIFY(config.allowMessageEditing());
    QVERIFY(config.otrEnabled());
    QVERIFY(!config.encryptionEnabled());
    QVERIFY(!config.needAdaptNewSubscriptionRC60());
    QCOMPARE(config.blockEditingMessageInMinutes(), 5);
    QCOMPARE(config.blockDeletingMessageInMinutes(), 5);
    QCOMPARE(config.ruqolaOauthTypes(), AuthenticationManager::OauthType::Password);
    QCOMPARE(config.serverOauthTypes(), AuthenticationManager::OauthType::Password);
    QVERIFY(!config.allowMessagePinning());
    QVERIFY(!config.allowMessageSnippeting());
    QVERIFY(!config.allowMessageStarring());
    QVERIFY(config.allowMessageDeleting());
    QVERIFY(!config.threadsEnabled());
    QVERIFY(!config.discussionEnabled());
    QVERIFY(!config.autoTranslateEnabled());
    QVERIFY(config.autoTranslateGoogleKey().isEmpty());
    QVERIFY(config.uploadFileEnabled());
    QCOMPARE(config.fileMaxFileSize(), -1);
    QVERIFY(!config.broadCastEnabled());
    QVERIFY(config.logoUrl().isEmpty());
    QVERIFY(config.faviconUrl().isEmpty());

    QVERIFY(config.videoRecorderEnabled());
    QVERIFY(config.audioRecorderEnabled());
    QVERIFY(config.allowDeleteOwnAccount());
    QVERIFY(config.registrationFromEnabled());
    QVERIFY(config.allowPasswordReset());
    QVERIFY(config.allowEmailChange());
    QVERIFY(config.allowPasswordChange());
    QVERIFY(config.allowUsernameChange());
}

void RuqolaServerConfigTest::shouldAssignValues()
{
    const QString jitsimeetprefix = QStringLiteral("test 1");
    const QString jitsimeeturl = QStringLiteral("test 2");
    const QString uniqueId = QStringLiteral("test 3");
    const QString filestoragetype = QStringLiteral("test 4");
    const QString googleKey = QStringLiteral("Google12");
    const bool jistsiEnabled = true;
    const bool allowEditing = false;
    const int minutes = 12;
    const int minutesDeletingMessage = 15;
    const bool otrEnable = false;
    const QString siteName = QStringLiteral("sitename");
    const QString siteUrl = QStringLiteral("siteurl");
    bool encryptionEnabled = false;
    bool autoTranslateEnabled = true;
    bool broadCastEnabled = true;
    const QString logoUrl = QStringLiteral("path/to/logo");
    const QString faviconUrl = QStringLiteral("path/to/favicon");
    const QString allowRegistrationFrom = QStringLiteral("Disabled");

    bool audioRecorderEnabled = false;
    bool videoRecorderEnabled = false;

    RuqolaServerConfig config;
    config.setJitsiMeetPrefix(jitsimeetprefix);
    config.setJitsiMeetUrl(jitsimeeturl);
    config.setUniqueId(uniqueId);
    config.setFileUploadStorageType(filestoragetype);
    config.setAllowMessageEditing(allowEditing);
    config.setBlockEditingMessageInMinutes(minutes);
    config.setBlockDeletingMessageInMinutes(minutesDeletingMessage);
    config.setOtrEnabled(otrEnable);
    config.setSiteName(siteName);
    config.setSiteUrl(siteUrl);
    config.setEncryptionEnabled(encryptionEnabled);
    config.setJitsiEnabled(jistsiEnabled);
    config.setAutoTranslateEnabled(autoTranslateEnabled);
    config.setAutoTranslateGoogleKey(googleKey);
    config.setBroadCastEnabled(broadCastEnabled);
    config.setAudioRecorderEnabled(audioRecorderEnabled);
    config.setVideoRecorderEnabled(videoRecorderEnabled);
    config.setLogoUrl(logoUrl);
    config.setFaviconUrl(faviconUrl);
    config.setAllowRegistrationFrom(allowRegistrationFrom);

    bool pinning = true;
    bool snippeting = true;
    bool starring = true;
    bool deleting = true;
    bool threadsEnabled = true;
    bool discussionEnabled = true;
    bool uploadFileEnabled = true;
    bool allowDeleteOwnAccount = false;
    bool allowPasswordReset = false;
    bool allowEmailChange = false;
    bool allowPasswordChange = false;
    bool allowUsernameChange = false;

    quint64 uploadFileMax = 222222;

    config.setAllowMessagePinning(pinning);
    config.setAllowMessageSnippeting(snippeting);
    config.setAllowMessageStarring(starring);
    config.setAllowMessageDeleting(deleting);
    config.setThreadsEnabled(threadsEnabled);
    config.setDiscussionEnabled(discussionEnabled);
    config.setUploadFileEnabled(uploadFileEnabled);
    config.setFileMaxFileSize(uploadFileMax);
    config.setAllowDeleteOwnAccount(allowDeleteOwnAccount);
    config.setAllowPasswordReset(allowPasswordReset);
    config.setAllowEmailChange(allowEmailChange);
    config.setAllowPasswordChange(allowPasswordChange);
    config.setAllowUsernameChange(allowUsernameChange);

    QCOMPARE(config.jitsiMeetPrefix(), jitsimeetprefix);
    QCOMPARE(config.jitsiMeetUrl(), jitsimeeturl);
    QCOMPARE(config.jitsiEnabled(), jistsiEnabled);
    QCOMPARE(config.uniqueId(), uniqueId);
    QCOMPARE(config.fileUploadStorageType(), filestoragetype);
    QCOMPARE(config.allowMessageEditing(), allowEditing);
    QCOMPARE(config.blockEditingMessageInMinutes(), minutes);
    QCOMPARE(config.blockDeletingMessageInMinutes(), minutesDeletingMessage);
    QCOMPARE(config.otrEnabled(), otrEnable);
    QCOMPARE(config.uploadFileEnabled(), uploadFileEnabled);
    QCOMPARE(config.fileMaxFileSize(), uploadFileMax);

    QCOMPARE(config.siteUrl(), siteUrl);
    QCOMPARE(config.siteName(), siteName);
    QCOMPARE(config.encryptionEnabled(), encryptionEnabled);
    QCOMPARE(config.broadCastEnabled(), broadCastEnabled);

    QCOMPARE(config.allowMessagePinning(), pinning);
    QCOMPARE(config.allowMessageSnippeting(), snippeting);
    QCOMPARE(config.allowMessageStarring(), starring);
    QCOMPARE(config.allowMessageDeleting(), deleting);
    QCOMPARE(config.threadsEnabled(), threadsEnabled);
    QCOMPARE(config.discussionEnabled(), discussionEnabled);
    QCOMPARE(config.autoTranslateEnabled(), autoTranslateEnabled);
    QCOMPARE(config.autoTranslateGoogleKey(), googleKey);

    QCOMPARE(config.audioRecorderEnabled(), audioRecorderEnabled);
    QCOMPARE(config.videoRecorderEnabled(), videoRecorderEnabled);

    QCOMPARE(config.logoUrl(), logoUrl);
    QCOMPARE(config.faviconUrl(), faviconUrl);

    QCOMPARE(config.allowDeleteOwnAccount(), allowDeleteOwnAccount);
    QCOMPARE(config.registrationFromEnabled(), false);
    QCOMPARE(config.allowPasswordReset(), allowPasswordReset);
    QCOMPARE(config.allowEmailChange(), allowEmailChange);
    QCOMPARE(config.allowPasswordChange(), allowPasswordChange);
    QCOMPARE(config.allowUsernameChange(), allowUsernameChange);
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
}

void RuqolaServerConfigTest::shouldEnabledRc60()
{
    QFETCH(QString, serverVersion);
    QFETCH(bool, needRc60);
    RuqolaServerConfig config;
    config.setServerVersion(serverVersion);
    //qDebug() << " config " << config;
    QCOMPARE(config.needAdaptNewSubscriptionRC60(), needRc60);
}

void RuqolaServerConfigTest::shouldVerifyOauthType_data()
{
    QTest::addColumn<QStringList>("oauthlist");
    QTest::addColumn<AuthenticationManager::OauthTypes>("types");

    {
        QStringList lst;
        //By default ruqola has password support
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
        QTest::newRow("unknow") << lst << types;
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
