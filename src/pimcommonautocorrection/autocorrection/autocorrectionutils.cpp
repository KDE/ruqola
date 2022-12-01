/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionutils.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
using namespace PimCommonAutoCorrection;
AutoCorrectionUtils::TypographicQuotes AutoCorrectionUtils::typographicDefaultSingleQuotes()
{
    TypographicQuotes quote;
    quote.begin = QChar(0x2018);
    quote.end = QChar(0x2019);
    return quote;
}

AutoCorrectionUtils::TypographicQuotes AutoCorrectionUtils::typographicDefaultDoubleQuotes()
{
    TypographicQuotes quote;
    quote.begin = QChar(0x201c);
    quote.end = QChar(0x201d);
    return quote;
}

AutoCorrectionUtils::TypographicQuotes AutoCorrectionUtils::typographicDefaultFrenchQuotes()
{
    TypographicQuotes quote;
    quote.begin = QChar(0x00AB);
    quote.end = QChar(0x00BB);
    return quote;
}

QDebug operator<<(QDebug d, PimCommonAutoCorrection::AutoCorrectionUtils::TypographicQuotes t)
{
    d << "TypographicQuotes.begin " << t.begin;
    d << "TypographicQuotes.end " << t.end;
    return d;
}

QString AutoCorrectionUtils::libreofficeFile(const QString &lang)
{
    return QStringLiteral("acor_%1.dat").arg(lang);
}

QStringList AutoCorrectionUtils::libreOfficeAutoCorrectionPath()
{
    QStringList dirList;
    auto maybeAddPath = [&dirList](const QString &path) {
        if (QFileInfo::exists(path)) {
            dirList.append(path);
        }
    };
    maybeAddPath(libreOfficeWritableLocalAutoCorrectionPath());
    maybeAddPath(libreOfficeSystemPath());
    return dirList;
}

QString AutoCorrectionUtils::libreOfficeSystemPath()
{
    // TODO add path for macos/windows etc.
#ifdef Q_OS_WIN
    return QStringLiteral("c:/Program Files/LibreOffice/share/autocorr/");
#else
    return QStringLiteral("/usr/lib64/libreoffice/share/autocorr/");
#endif
}

QString AutoCorrectionUtils::libreOfficeLocalPath()
{
    return QStringLiteral("/libreoffice/4/user/autocorr/");
}

QString AutoCorrectionUtils::libreOfficeWritableLocalAutoCorrectionPath()
{
#ifdef Q_OS_WIN
    const QString writeablePath =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).remove(QStringLiteral("ruqola")) + AutoCorrectionUtils::libreOfficeLocalPath();
    return writeablePath;
#else
    return QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + libreOfficeLocalPath();
#endif
}

QStringList AutoCorrectionUtils::searchAutoCorrectLibreOfficeFiles()
{
    QStringList files;
    const QString path = libreOfficeSystemPath();
    if (QFileInfo::exists(path)) {
        QDir dir(path);
        const QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString &file : entryList) {
            QString curFile = file;
            curFile.remove(path);
            curFile.remove(QStringLiteral(".dat"));
            curFile.remove(QStringLiteral("acor_"));
            files.append(curFile);
        }
    }
    return files;
}

QStringList AutoCorrectionUtils::autoCorrectLibreOfficeLanguageToString(const QStringList &langs)
{
    QStringList languagesStr;
    const QList<QLocale> allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
    for (const QLocale &locale : allLocales) {
        const QString languageCode = locale.name();
        for (const QString &lang : langs) {
            if (languageCode == lang) {
                const QString nativeName = locale.nativeLanguageName();
                // For some languages the native name might be empty.
                // In this case use the non native language name as fallback.
                // See: QTBUG-51323
                const QString languageName = nativeName.isEmpty() ? QLocale::languageToString(locale.language()) : nativeName;
                languagesStr.append(languageName);
                break;
            } else {
                QString b = lang;
                b.replace(QLatin1Char('-'), QLatin1Char('_'));
                if (languageCode == b) {
                    const QString nativeName = locale.nativeLanguageName();
                    // For some languages the native name might be empty.
                    // In this case use the non native language name as fallback.
                    // See: QTBUG-51323
                    const QString languageName = nativeName.isEmpty() ? QLocale::languageToString(locale.language()) : nativeName;
                    languagesStr.append(languageName);
                    break;
                }
            }
        }
    }
    return languagesStr;
}

QString AutoCorrectionUtils::TypographicQuotes::toString() const
{
    return begin + end;
}

bool AutoCorrectionUtils::TypographicQuotes::isEmpty() const
{
    return begin.isNull() && end.isNull();
}

AutoCorrectionUtils::TypographicQuotes AutoCorrectionUtils::TypographicQuotes::fromString(const QString &str)
{
    AutoCorrectionUtils::TypographicQuotes quotes;
    if (str.size() == 2) {
        quotes.begin = str.at(0);
        quotes.end = str.at(1);
    }
    return quotes;
}

QString AutoCorrectionUtils::containsAutoCorrectionFile(const QString &lang)
{
    const QStringList libreOfficeAutocorrectPaths = AutoCorrectionUtils::libreOfficeAutoCorrectionPath();
    if (!libreOfficeAutocorrectPaths.isEmpty()) {
        QString fixLangExtension = lang;
        fixLangExtension.replace(QLatin1Char('_'), QLatin1Char('-'));
        for (const auto &path : libreOfficeAutocorrectPaths) {
            const QString filename = path + AutoCorrectionUtils::libreofficeFile(fixLangExtension);
            // qDebug() << " filename " << filename;
            if (QFileInfo::exists(filename)) {
                return filename;
            }
        }
    }
    return {};
}

QStringList AutoCorrectionUtils::wordsFromSentence(const QString &string)
{
    QStringList lst;
    if (!string.trimmed().isEmpty()) {
        lst.append(string);
        QString tmpString = string;
        while (!tmpString.trimmed().isEmpty()) {
            bool foundStr = false;
            for (auto i = 0; i < tmpString.count(); i++) {
                if (tmpString.at(i).isSpace()) {
                    QString value;
                    const auto pos = tmpString.count() - i - 1;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    value = tmpString.right(pos);
                    // qDebug() << " value !!! " << value;
#else
                    value = tmpString.last(pos);
#endif
                    if (!value.trimmed().isEmpty()) {
                        lst.append(value);
                    }
                    tmpString = value;
                    foundStr = true;
                    break;
                }
            }
            if (!foundStr) {
                break;
            }
        }
    }
    return lst;
}
