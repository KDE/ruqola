/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationmanagertest.h"
#include "restauthenticationmanager.h"
#include "rocketchataccount.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(RESTAuthenticationManagerTest)
RESTAuthenticationManagerTest::RESTAuthenticationManagerTest(QObject *parent)
    : QObject(parent)
{
}

#if 0
void RESTAuthenticationManagerTest::testLoginSuccess()
{
    // Logging in with an existing token
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManager authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
        authManager.setAuthToken(QStringLiteral("some token"));
        authManager.login();
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManager authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
        authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in using oauth
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManager authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
        authManager.loginOAuth(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManager authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
        authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }

    // Logging in with OAuth
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManager authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
        authManager.loginOAuth(QStringLiteral("sometoken"), QStringLiteral("somesecret"));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), QStringLiteral("some id"));
        QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
    }
}

void RESTAuthenticationManagerTest::testLoginInvalidLoginInfo()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword);
}

void RESTAuthenticationManagerTest::testLoginWithOtpSuccess()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpRequired);

    authManager.sendOTP(QStringLiteral("otpcode"));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

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
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedIn);
    QCOMPARE(authManager.userId(), QStringLiteral("some id"));
    QCOMPARE(authManager.authToken(), QStringLiteral("some token"));
}

void RESTAuthenticationManagerTest::testLoginWithOtpFailure()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("wronguser"), QStringLiteral("wrongpassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

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
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpRequired);

    authManager.sendOTP(QStringLiteral("otpcode"));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

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
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedInvalidOtp);
}

void RESTAuthenticationManagerTest::testUnknownError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "unknown-error"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::GenericError);
}

void RESTAuthenticationManagerTest::testUserNotActivatedError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-user-is-not-activated"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedUserNotActivated);
}

void RESTAuthenticationManagerTest::testLoginBlockForIpError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-ip"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginBlockForIp);
}

void RESTAuthenticationManagerTest::testLoginBlockedForUser()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-user"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginBlockedForUser);
}

void RESTAuthenticationManagerTest::testLoginAppUserAllowToLogin()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManager authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManager::loginStatusChanged);
    authManager.login(QStringLiteral("someuser"), QStringLiteral("somepassword"));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-app-user-is-not-allowed-to-login"
        }
    })")));

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginAppNotAllowedToLogin);
}
#endif
#include "moc_restauthenticationmanagertest.cpp"
