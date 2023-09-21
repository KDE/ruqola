/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"

#include <QJsonArray>

namespace DDPAuthenticationManagerUtils
{
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray loginResume(const QString &token);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray loginLdap(const QString &user, const QString &password);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray loginOAuth(const QString &credentialToken, const QString &credentialSecret);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray login(const QString &user, const QString &password);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonArray sendOTP(const QString &otpCode, const QJsonObject &lastLoginPayload);
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QJsonObject hashPassword(const QString &password);
};
