/*

 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

        authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

        authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

        authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

        authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

        authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

    authManager.processMethodResponse(1, Utils::strToJsonObject(QStringLiteral(R"(
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

    authManager.processMethodResponse(0, Utils::strToJsonObject(QStringLiteral(R"(
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

    authManager.processMethodResponse(1, Utils::strToJsonObject(QStringLiteral(R"(
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
