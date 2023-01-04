/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineutil.h"

QString LibreTranslateEngineUtil::groupName()
{
    return QStringLiteral("LibreTranslateTranslator");
}

QString LibreTranslateEngineUtil::serverUrlKey()
{
    return QStringLiteral("ServerUrl");
}

QString LibreTranslateEngineUtil::serverRequiredApiKey()
{
    return QStringLiteral("RequiredApiKey");
}

QString LibreTranslateEngineUtil::defaultServerUrl()
{
    return QStringLiteral("https://libretranslate.com");
}
