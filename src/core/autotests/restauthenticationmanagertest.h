/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RESTAuthenticationManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit RESTAuthenticationManagerTest(QObject *parent = nullptr);
    ~RESTAuthenticationManagerTest() override = default;

private Q_SLOTS:
    void testLoginSuccess();
    void testLoginInvalidLoginInfo();
    void testLoginWithOtpSuccess();
    void testLoginWithOtpFailure();
    void testUnknownError();
    void testUserNotActivatedError();
    void testLoginBlockForIpError();
    void testLoginBlockedForUser();
    void testLoginAppUserAllowToLogin();
};
