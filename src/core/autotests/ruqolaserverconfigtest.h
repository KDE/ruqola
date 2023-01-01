/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RuqolaServerConfigTest : public QObject
{
    Q_OBJECT
public:
    explicit RuqolaServerConfigTest(QObject *parent = nullptr);
    ~RuqolaServerConfigTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldAssignValues();

    void shouldEnabledRc60_data();
    void shouldEnabledRc60();

    void shouldVerifyOauthType_data();
    void shouldVerifyOauthType();

    void shouldVerifyThatServerSupportService();

    void shouldAddRuqolaAuthenticationSupport();

    void shouldTestVersion_data();
    void shouldTestVersion();
};
