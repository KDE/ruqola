/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QString>

namespace LibreTranslateEngineUtil
{
Q_REQUIRED_RESULT QString groupName();
Q_REQUIRED_RESULT QString serverUrlKey();
Q_REQUIRED_RESULT QString defaultServerUrl();
Q_REQUIRED_RESULT QString serverRequiredApiKey();
Q_REQUIRED_RESULT QString adaptUrl(QString serverUrl);
Q_REQUIRED_RESULT QString apiGroupName();
};
