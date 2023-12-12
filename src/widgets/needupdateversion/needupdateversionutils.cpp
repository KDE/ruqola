/*
  SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  code based on kdenlive
*/

#include "needupdateversionutils.h"

#include <KConfigGroup>
#include <KSharedConfig>

#include <QRegularExpression>

NeedUpdateVersionUtils::ObsoleteVersion NeedUpdateVersionUtils::obsoleteVersionStatus(const QString &str, const QDate &currentDate)
{
    static QRegularExpression regular{QStringLiteral("\\((.*)\\)")};
    QRegularExpressionMatch match;
    QString captured;
    if (str.contains(regular, &match)) {
        captured = match.captured(1);
    } else {
        captured = str;
    }
    if (!captured.isEmpty()) {
        const QStringList version = captured.split(QLatin1Char('.'));
        if (version.size() > 2) {
            bool ok;
            int year = version.at(0).toInt(&ok);
            if (ok) {
                const int month = version.at(1).toInt(&ok);
                if (ok) {
                    if (year < 100) {
                        year += 2000;
                    }
                    const QDate releaseDate = QDate(year, month, 1);
                    if (releaseDate.isValid()) {
                        const int days = releaseDate.daysTo(currentDate);
                        if (days > 180) {
                            if (days > 360) {
                                return NeedUpdateVersionUtils::ObsoleteVersion::OlderThan12Months;
                            }
                            return NeedUpdateVersionUtils::ObsoleteVersion::OlderThan6Months;
                        }
                    }
                }
            }
        }
    } else {
        return NeedUpdateVersionUtils::ObsoleteVersion::Unknown;
    }
    return NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet;
}

void NeedUpdateVersionUtils::disableCheckVersion()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, QStringLiteral("Check Version"));
    group.writeEntry("checkerVersionEnabled", false);
}

bool NeedUpdateVersionUtils::checkVersion()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, QStringLiteral("Check Version"));
    return group.readEntry("checkerVersionEnabled", true);
}
