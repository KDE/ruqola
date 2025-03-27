/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDate>
#include <QMetaType>
#include <QString>
namespace NeedUpdateVersionUtils
{
enum class LIBRUQOLAWIDGETS_TESTS_EXPORT ObsoleteVersion : uint8_t {
    Unknown,
    NotObsoleteYet,
    OlderThan6Months,
    OlderThan12Months,
};

void disableCheckVersion();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT bool checkVersion();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT NeedUpdateVersionUtils::ObsoleteVersion obsoleteVersionStatus(const QString &str, const QDate &currentDate);
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QUrl newVersionUrl();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT bool canVerifyNewVersion();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QDate compileDate();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QUrl nextVersionUrl();
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QUrl generateNewVersionUrl(const QString &currentStableVersion);
};
Q_DECLARE_METATYPE(NeedUpdateVersionUtils::ObsoleteVersion)
