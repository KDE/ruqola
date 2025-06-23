/*
  SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  code based on kdenlive
*/

#include "needupdateversionutils.h"
#include "config-ruqola.h"

#include <KConfigGroup>
#include <KSharedConfig>

#include <QRegularExpression>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
NeedUpdateVersionUtils::ObsoleteVersion NeedUpdateVersionUtils::obsoleteVersionStatus(const QString &str, const QDate &currentDate)
{
    static const QRegularExpression regular{u"\\((.*)\\)"_s};
    QRegularExpressionMatch match;
    QString captured;
    if (str.contains(regular, &match)) {
        captured = match.captured(1);
    } else {
        captured = str;
    }
    if (!captured.isEmpty()) {
        const QStringList version = captured.split(u'.');
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
    KConfigGroup group(config, u"Check Version"_s);
    group.writeEntry("checkerVersionEnabled", false);
}

bool NeedUpdateVersionUtils::checkVersion()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group(config, u"Check Version"_s);
    return group.readEntry("checkerVersionEnabled", true);
}

QDate NeedUpdateVersionUtils::compileDate()
{
    return QDate::fromString(QString::fromLatin1(__DATE__), u"MMM dd yyyy"_s);
}

QUrl NeedUpdateVersionUtils::nextVersionUrl()
{
    // For stable version we need to look at if we have next stable version.
    return generateNewVersionUrl("2.7"_L1);
}

QUrl NeedUpdateVersionUtils::newVersionUrl()
{
    return generateNewVersionUrl("2.6"_L1);
}

QUrl NeedUpdateVersionUtils::generateNewVersionUrl([[maybe_unused]] const QString &currentStableVersion)
{
    // TODO add support for AppImage linux too
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    const QString baseUrl = "https://cdn.kde.org/ci-builds/network/ruqola/"_L1;
#if RUQOLA_STABLE_VERSION
    QString url = baseUrl + currentStableVersion + "/%1/"_L1;
#else
    QString url = baseUrl + "master/%1/"_L1;
#endif
#endif

#if defined(Q_OS_WIN)
    url = url.arg("windows"_L1);
    return QUrl(url);
#endif

#if defined(Q_OS_MACOS)
#ifdef Q_PROCESSOR_ARM_64
    url = url.arg("macos-arm64"_L1);
    return QUrl(url);
#else
    url = url.arg("macos-x86_64"_L1);
    return QUrl(url);
#endif
#endif
    return {};
}

bool NeedUpdateVersionUtils::canVerifyNewVersion()
{
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    return true;
#endif
    return false;
}
