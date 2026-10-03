/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationmanagerutilstest.h"
#include "authenticationmanager/authenticationmanagerutils.h"
#include <QJsonArray>
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
        const QString token = uR"(dsdf##56;)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), "[{\"resume\":\"dsdf##56;\"}]"_ba);
    }

    {
        const QString token = uR"(foo42";)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginResume(token)).toJson(QJsonDocument::Compact), "[{\"resume\":\"foo42\\\";\"}]"_ba);
    }
}

void AuthenticationManagerUtilsTest::shouldTestLdapLogin()
{
    {
        const QString user = uR"(username42)"_s;
        const QString password = uR"(blafoo4)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 "[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"blafoo4\",\"username\":\"username42\"}]"_ba);
    }
    {
        const QString user = uR"(username42)"_s;
        const QString password = uR"(45p";)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginLdap(user, password)).toJson(QJsonDocument::Compact),
                 "[{\"ldap\":true,\"ldapOptions\":{},\"ldapPass\":\"45p\\\";\",\"username\":\"username42\"}]"_ba);
    }
}

void AuthenticationManagerUtilsTest::shouldTestloginOAuth()
{
    {
        const QString credentialToken = uR"(username42)"_s;
        const QString credentialSecret = uR"(blafoo4)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::loginOAuth(credentialToken, credentialSecret)).toJson(QJsonDocument::Compact),
                 "[{\"oauth\":{\"credentialSecret\":\"blafoo4\",\"credentialToken\":\"username42\"}}]"_ba);
    }
}

void AuthenticationManagerUtilsTest::shouldTestlogin()
{
    {
        const QString user = uR"(username42)"_s;
        const QString password = uR"(45p";)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"username\":\"username42\"}}]"));
    }
    {
        // Use email as login
        const QString user = uR"(foo@kde.org)"_s;
        const QString password = uR"(45p";)"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::login(user, password)).toJson(QJsonDocument::Compact),
                 QByteArray("[{\"password\":{\"algorithm\":\"sha-256\",\"digest\":\"465906e4251bcc0e47e97be030e468a3dcc011eb422b12c2a6ddba35d76f9df8\"},"
                            "\"user\":{\"email\":\"foo@kde.org\"}}]"));
    }
}

void AuthenticationManagerUtilsTest::shouldTestSendOtp()
{
    {
        const QString codeOtp = uR"(foo42)"_s;
        QJsonObject lastLoginPayLoad;
        lastLoginPayLoad["test"_L1] = u"test"_s;
        QCOMPARE(QJsonDocument(AuthenticationManagerUtils::sendOTP(codeOtp, lastLoginPayLoad)).toJson(QJsonDocument::Compact),
                 "[{\"totp\":{\"code\":\"foo42\",\"login\":{\"test\":\"test\"}}}]"_ba);
    }
}

#include "moc_authenticationmanagerutilstest.cpp"
