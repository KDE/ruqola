/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaengineutil.h"

QString LingvaEngineUtil::groupName()
{
    return QStringLiteral("LingvaTranslator");
}

QString LingvaEngineUtil::serverUrlKey()
{
    return QStringLiteral("ServerUrl");
}

QString LingvaEngineUtil::defaultServerUrl()
{
    return QStringLiteral("https://lingva.ml");
}
