/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanagerutilstest.h"
#include "authenticationmanager/authenticationmanagerutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(AuthenticationManagerUtilsTest)
AuthenticationManagerUtilsTest::AuthenticationManagerUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void AuthenticationManagerUtilsTest::shouldTestLoginResume()
{
    {
        const QString token = QStringLiteral(R"(dsdf##56;)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"dsdf##56;\"}]"));
    }

    {
        const QString token = QStringLiteral(R"(foo42";)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"foo42\\\";\"}]"));
    }
}

void AuthenticationManagerUtilsTest::shouldTestLdapLogin()
{
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"blafoo4\",\"username\":\"username42\"}]"));
    }
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"45p\\\";\",\"username\":\"username42\"}]"));
    }
}

void AuthenticationManagerUtilsTest::shouldTestloginOAuth()
{
    {
        const QString credentialToken = QStringLiteral(R"(username42)");
        const QString credentialSecret = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"oauth\":{\"credentialSecret\":\"blafoo4\",\"credentialToken\":\"username42\"}}]"));
    }
}

void AuthenticationManagerUtilsTest::shouldTestlogin()
{
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"username\":\"username42\"}}]"));
    }
    {
        // Use email as login
        const QString user = QStringLiteral(R"(foo@kde.org)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"email\":\"foo@kde.org\"}}]"));
    }
}

void AuthenticationManagerUtilsTest::shouldTestSendOtp()
{
    {
        const QString codeOtp = QStringLiteral(R"(foo42)");
        QJsonObject lastLoginPayLoad;
        lastLoginPayLoad["test"_L1] = u"test"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::sendOTP(codeOtp, lastLoginPayLoad)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"totp\":{\"code\":\"foo42\",\"login\":{\"test\":\"test\"}}}]"));
    }
}

#include "moc_authenticationmanagerutilstest.cpp"
