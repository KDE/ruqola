/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpauthenticationmanagertest.h"

#include "rocketchataccount.h"
#include "utils.h"

#include "ddpapi/ddpauthenticationmanager.h"

#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(DDPAuthenticationManagerTest)

DDPAuthenticationManagerTest::DDPAuthenticationManagerTest(QObject *parent)
    : QObject(parent)
{
}

void DDPAuthenticationManagerTest::testLoginSuccess()
{
    // Logging in with an existing token
    {
        RocketChatAccount dummyAccount;
        DDPAuthenticationManager authManager(dummyAccount.ddp());

        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
        authManager.setAuthToken(QStringLiteral("some token"));
        authManager.login();
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponse(0,
                                          Utils::strToJsonObject(QStringLiteral(
                                              R"(
        {
            "msg": "result",
            "id": "0",
            "result": {
                "id": "some id",
                "token": "some token",
                "tokenExpires": {
                    "$date": 1590844250110
                },
                "type": "resume"
            }
        })")));

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        DDPAuthenticationManager authManager(dummyAccount.ddp());

        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
        authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponse(0,
                                          Utils::strToJsonObject(QStringLiteral(
                                              R"(
        {
            "msg": "result",
            "id": "0",
            "result": {
                "id": "some id",
                "token": "some token",
                "tokenExpires": {
                    "$date": 1590844250110
                },
                "type": "?????"
            }
        })"))); // TODO: check the type for non-ldap, non-2fa successful login messages

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in using oauth
    {
        RocketChatAccount dummyAccount;
        DDPAuthenticationManager authManager(dummyAccount.ddp());

        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
        authManager.loginOAuth(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponse(0,
                                          Utils::strToJsonObject(QStringLiteral(
                                              R"(
        {
            "msg": "result",
            "id": "0",
            "result": {
                "id": "some id",
                "token": "some token",
                "tokenExpires": {
                    "$date": 1590844250110
                },
                "type": "?????"
            }
        })"))); // TODO: check the type for non-ldap, non-2fa successful login messages

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        DDPAuthenticationManager authManager(dummyAccount.ddp());

        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
        authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponse(0,
                                          Utils::strToJsonObject(QStringLiteral(
                                              R"(
        {
            "msg": "result",
            "id": "0",
            "result": {
                "id": "some id",
                "token": "some token",
                "tokenExpires": {
                    "$date": 1590844250110
                },
                "type": "ldap"
            }
        })")));

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with OAuth
    {
        RocketChatAccount dummyAccount;
        DDPAuthenticationManager authManager(dummyAccount.ddp());

        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
        authManager.loginOAuth(QStringLiteral("sometoken"), QStringLiteral("somesecret"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponse(0,
                                          Utils::strToJsonObject(QStringLiteral(
                                              R"(
        {
            "msg": "result",
            "id": "0",
            "result": {
                "id": "some id",
                "token": "some token",
                "tokenExpires": {
                    "$date": 1590844250110
                },
                "type": "????"
            }
        })")));

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }
}

void DDPAuthenticationManagerTest::testLoginInvalidLoginInfo()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0,
                                      Utils::strToJsonObject(QStringLiteral(
                                          R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "isClientSafe": true,
            "error": 403,
            "reason": "User not found",
            "message": "User not found [403]",
            "errorType": "Meteor.Error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword);
}

void DDPAuthenticationManagerTest::testLoginWithOtpSuccess()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0,
                                      Utils::strToJsonObject(QStringLiteral(
                                          R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "isClientSafe": true,
            "error": "totp-required",
            "reason": "TOTP Required",
            "details": {
                "method": "email",
                "codeGenerated": true,
                "availableMethods": [
                    "email"
                ]
            },
            "message": "TOTP Required [totp-required]",
            "errorType": "Meteor.Error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOtpRequired);

    authManager.sendOTP(QStringLiteral("otpcode"));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

    authManager.processMethodResponse(1,
                                      Utils::strToJsonObject(QStringLiteral(
                                          R"(
    {
        "msg": "result",
        "id": "1",
        "result": {
            "id": "some id",
            "token": "some token",
            "tokenExpires": {
                "$date": 1596139026776
            },
            "type": "password"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 4);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedIn);
    QCOMPARE(authManager.userId(), QStringLiteral("some id"));
    QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
}

void DDPAuthenticationManagerTest::testLoginWithOtpFailure()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0,
                                      Utils::strToJsonObject(QStringLiteral(
                                          R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "isClientSafe": true,
            "error": "totp-required",
            "reason": "TOTP Required",
            "details": {
                "method": "email",
                "codeGenerated": true,
                "availableMethods": [
                    "email"
                ]
            },
            "message": "TOTP Required [totp-required]",
            "errorType": "Meteor.Error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOtpRequired);

    authManager.sendOTP(QStringLiteral("otpcode"));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

    authManager.processMethodResponse(1,
                                      Utils::strToJsonObject(QStringLiteral(
                                          R"(
    {
        "msg": "result",
        "id": "1",
        "error": {
            "isClientSafe": true,
            "error": "totp-invalid",
            "reason": "TOTP Invalid",
            "details": {
                "method": "email"
            },
            "message": "TOTP Invalid [totp-invalid]",
            "errorType": "Meteor.Error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 4);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedInvalidOtp);
}

void DDPAuthenticationManagerTest::testUnknownError()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "unknown-error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::GenericError);
}

void DDPAuthenticationManagerTest::testUserNotActivatedError()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-user-is-not-activated"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedUserNotActivated);
}

void DDPAuthenticationManagerTest::testLoginBlockForIpError()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-ip"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedLoginBlockForIp);
}

void DDPAuthenticationManagerTest::testLoginBlockedForUser()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-user"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedLoginBlockedForUser);
}

void DDPAuthenticationManagerTest::testLoginAppUserAllowToLogin()
{
    RocketChatAccount dummyAccount;
    DDPAuthenticationManager authManager(dummyAccount.ddp());

    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &DDPAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-app-user-is-not-allowed-to-login"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), DDPAuthenticationManager::LoginStatus::LoginFailedLoginAppNotAllowedToLogin);
}

#include "moc_ddpauthenticationmanagertest.cpp"
