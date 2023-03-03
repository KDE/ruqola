/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class DDPAuthenticationManagerUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit DDPAuthenticationManagerUtilsTest(QObject *parent = nullptr);
    ~DDPAuthenticationManagerUtilsTest() override = default;

private Q_SLOTS:
    void shouldTestLoginResume();
    void shouldTestLdapLogin();
    void shouldTestloginOAuth();
    void shouldTestlogin();
    void shouldTestSendOtp();
};
