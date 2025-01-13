/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QString>

namespace NeedUpdateParseHtmlUtil
{
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QString extractDate(const QString &dataHtml);
};
