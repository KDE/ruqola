/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AuthenticationManagerUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit AuthenticationManagerUtilsTest(QObject *parent = nullptr);
    ~AuthenticationManagerUtilsTest() override = default;

private Q_SLOTS:
    void shouldTestLoginResume();
    void shouldTestLdapLogin();
    void shouldTestloginOAuth();
    void shouldTestlogin();
    void shouldTestSendOtp();
};
