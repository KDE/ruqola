/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ddpauthenticationmanagerutilstest.h"
#include "ddpapi/ddpauthenticationmanagerutils.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(DDPAuthenticationManagerUtilsTest)
DDPAuthenticationManagerUtilsTest::DDPAuthenticationManagerUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void DDPAuthenticationManagerUtilsTest::shouldTestLoginResume()
{
    {
        const QString token = QStringLiteral(R"(dsdf##56;)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"dsdf##56;\"}]"));
    }

    {
        const QString token = QStringLiteral(R"(foo42";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), QByteArray("[{\"resume\":\"foo42\\\";\"}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestLdapLogin()
{
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"blafoo4\",\"username\":\"username42\"}]"));
    }
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"45p\\\";\",\"username\":\"username42\"}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestloginOAuth()
{
    {
        const QString credentialToken = QStringLiteral(R"(username42)");
        const QString credentialSecret = QStringLiteral(R"(blafoo4)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"oauth\":{\"credentialSecret\":\"blafoo4\",\"credentialToken\":\"username42\"}}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestlogin()
{
    {
        const QString user = QStringLiteral(R"(username42)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"username\":\"username42\"}}]"));
    }
    {
        // Use email as login
        const QString user = QStringLiteral(R"(foo@kde.org)");
        const QString password = QStringLiteral(R"(45p";)");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"email\":\"foo@kde.org\"}}]"));
    }
}

void DDPAuthenticationManagerUtilsTest::shouldTestSendOtp()
{
    {
        const QString codeOtp = QStringLiteral(R"(foo42)");
        QJsonObject lastLoginPayLoad;
        lastLoginPayLoad[QStringLiteral("test")] = QStringLiteral("test");
        QCOMPARE(QJsonDocument(DDPAuthenticationManagerUtils::sendOTP(codeOtp, lastLoginPayLoad)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"totp\":{\"code\":\"foo42\",\"login\":{\"test\":\"test\"}}}]"));
    }
}
