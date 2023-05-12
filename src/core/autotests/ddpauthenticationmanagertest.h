/*

 * SPDX-FileCopyrightText: 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 *
 */

#pragma once

#include <QObject>

class DDPAuthenticationManagerTest : public QObject
{
    Q_OBJECT

public:
    explicit DDPAuthenticationManagerTest(QObject *parent = nullptr);
    ~DDPAuthenticationManagerTest() override = default;

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
