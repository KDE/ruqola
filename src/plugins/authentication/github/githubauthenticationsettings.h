/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QUrl>

namespace GitHubAuthenticationSettings
{
[[nodiscard]] QString clientId();
[[nodiscard]] QString clientSecret();
[[nodiscard]] QUrl authorizeUrl();
[[nodiscard]] QUrl accessTokenUrl();
};
