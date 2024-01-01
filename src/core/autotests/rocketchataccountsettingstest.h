/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RocketChatAccountSettingsTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatAccountSettingsTest(QObject *parent = nullptr);
    ~RocketChatAccountSettingsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSignalWhenSetServerURLChanged();
    void shouldEmitSignalWhenUserNameChanged();
    void shouldEmitSignalWhenUserIDChanged();
    void shouldEmitSignalWhenLoginStatusChanged();
    void shouldSetAccountName();
    void shouldSetUserID();
    void shouldsetAuthToken();
    void shouldSetServerUrl();
    void shouldSetUserName();
    void shouldSetPassword();
    void shouldLogout();
    void shouldNotEmitSignalWhenNewUsernameIsSameAsOld();
    void shouldNotEmitSignalWhenNewServerUrlIsSameAsOld();
};
