/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restauthenticationmanagertest.h"
using namespace Qt::Literals::StringLiterals;

#include "authenticationmanager/restauthenticationmanager.h"
#include "rocketchataccount.h"
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>

class RESTAuthenticationManagerTestImpl : public RESTAuthenticationManager
{
public:
    explicit RESTAuthenticationManagerTestImpl(Connection *restApiConnection, QObject *parent = nullptr)
        : RESTAuthenticationManager(restApiConnection, parent)
    {
    }
    ~RESTAuthenticationManagerTestImpl() override = default;

protected:
    void callLoginImpl(const QJsonArray &params, RESTAuthenticationManagerTestImpl::Method method) override
    {
        Q_UNUSED(params);
        Q_UNUSED(method);
        // Nothing
    }
};

QTEST_GUILESS_MAIN(RESTAuthenticationManagerTest)
RESTAuthenticationManagerTest::RESTAuthenticationManagerTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RESTAuthenticationManagerTest::testLoginSuccess()
{
    // Logging in with an existing token
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
        authManager.setAuthToken(u"some token"_s);
        QVERIFY(authManager.login());
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
        })")),
                                              RESTAuthenticationManager::Method::Login);

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), u"some id"_s);
        QCOMPARE(authManager.authToken(), u"some token"_s);
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
        QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
        })")),
                                              RESTAuthenticationManager::Method::Login); // TODO: check the type for non-ldap, non-2fa successful login messages

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), u"some id"_s);
        QCOMPARE(authManager.authToken(), u"some token"_s);
    }

    // Logging in using oauth
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
        QVERIFY(authManager.loginOAuth(u"someuser"_s, u"somepassword"_s));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
        })")),
                                              RESTAuthenticationManager::Method::Login); // TODO: check the type for non-ldap, non-2fa successful login messages

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), u"some id"_s);
        QCOMPARE(authManager.authToken(), u"some token"_s);
    }

    // Logging in with username and password
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
        QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
        })")),
                                              RESTAuthenticationManager::Method::Login);

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), u"some id"_s);
        QCOMPARE(authManager.authToken(), u"some token"_s);
    }

    // Logging in with OAuth
    {
        RocketChatAccount dummyAccount;
        RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
        QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
        QVERIFY(authManager.loginOAuth(u"sometoken"_s, u"somesecret"_s));
        QCOMPARE(spyStatusChanged.count(), 1);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

        authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
        })")),
                                              RESTAuthenticationManager::Method::Login);

        QCOMPARE(spyStatusChanged.count(), 2);
        QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoggedIn);
        QCOMPARE(authManager.userId(), u"some id"_s);
        QCOMPARE(authManager.authToken(), u"some token"_s);
    }
}

void RESTAuthenticationManagerTest::testLoginInvalidLoginInfo()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"wronguser"_s, u"wrongpassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedInvalidUserOrPassword);
}

void RESTAuthenticationManagerTest::testLoginWithOtpSuccess()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"wronguser"_s, u"wrongpassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpRequired);

    QVERIFY(authManager.sendOTP(u"otpcode"_s));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
    })")),
                                          RESTAuthenticationManager::Method::SendOtp);

    QCOMPARE(spyStatusChanged.count(), 4);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedIn);
    QCOMPARE(authManager.userId(), u"some id"_s);
    QCOMPARE(authManager.authToken(), u"some token"_s);
}

void RESTAuthenticationManagerTest::testLoginWithOtpFailure()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"wronguser"_s, u"wrongpassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpRequired);

    QVERIFY(authManager.sendOTP(u"otpcode"_s));
    QCOMPARE(spyStatusChanged.count(), 3);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOtpAuthOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(
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
    })")),
                                          RESTAuthenticationManager::Method::SendOtp);

    QCOMPARE(spyStatusChanged.count(), 4);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedInvalidOtp);
}

void RESTAuthenticationManagerTest::testUnknownError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "unknown-error"
        }
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::GenericError);
}

void RESTAuthenticationManagerTest::testUserNotActivatedError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-user-is-not-activated"
        }
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedUserNotActivated);
}

void RESTAuthenticationManagerTest::testLoginBlockForIpError()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-ip"
        }
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginBlockForIp);
}

void RESTAuthenticationManagerTest::testLoginBlockedForUser()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-login-blocked-for-user"
        }
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginBlockedForUser);
}

void RESTAuthenticationManagerTest::testLoginAppUserAllowToLogin()
{
    RocketChatAccount dummyAccount;
    RESTAuthenticationManagerTestImpl authManager(dummyAccount.restApi());

    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoggedOut);
    QSignalSpy spyStatusChanged(&authManager, &RESTAuthenticationManagerTestImpl::loginStatusChanged);
    QVERIFY(authManager.loginPassword(u"someuser"_s, u"somepassword"_s));
    QCOMPARE(spyStatusChanged.count(), 1);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginOngoing);

    authManager.processMethodResponseImpl(Utils::strToJsonObject(QStringLiteral(R"(
    {
        "msg": "result",
        "id": "0",
        "error": {
            "error": "error-app-user-is-not-allowed-to-login"
        }
    })")),
                                          RESTAuthenticationManager::Method::Login);

    QCOMPARE(spyStatusChanged.count(), 2);
    QCOMPARE(authManager.loginStatus(), AuthenticationManager::LoginStatus::LoginFailedLoginAppNotAllowedToLogin);
}

#include "moc_restauthenticationmanagertest.cpp"
