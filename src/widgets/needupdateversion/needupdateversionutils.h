/*
  SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDate>
#include <QMetaType>
#include <QString>
namespace NeedUpdateVersionUtils
{
enum class LIBRUQOLAWIDGETS_TESTS_EXPORT ObsoleteVersion {
    Unknown,
    NotObsoleteYet,
    OlderThan6Months,
    OlderThan12Months,
};

void disableCheckVersion();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT bool checkVersion();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateVersionUtils::ObsoleteVersion obsoleteVersionStatus(const QString &str, const QDate &currentDate);
};
Q_DECLARE_METATYPE(NeedUpdateVersionUtils::ObsoleteVersion)
