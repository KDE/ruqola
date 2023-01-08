/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "textautocorrection_export.h"
#include <QChar>
#include <QDebug>
namespace TextAutoCorrection
{
namespace AutoCorrectionUtils
{
struct TEXTAUTOCORRECTION_EXPORT TypographicQuotes {
    QChar begin;
    QChar end;
    Q_REQUIRED_RESULT QString toString() const;
    Q_REQUIRED_RESULT bool isEmpty() const;
    Q_REQUIRED_RESULT static TypographicQuotes fromString(const QString &str);
};

Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT TypographicQuotes typographicDefaultSingleQuotes();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT TypographicQuotes typographicDefaultDoubleQuotes();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT TypographicQuotes typographicDefaultFrenchQuotes();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QString libreofficeFile(const QString &lang);
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QStringList libreOfficeAutoCorrectionPath();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QStringList searchAutoCorrectLibreOfficeFiles();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QStringList autoCorrectLibreOfficeLanguageToString(const QStringList &langs);
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QString libreOfficeWritableLocalAutoCorrectionPath();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QString containsAutoCorrectionFile(const QString &lang,
                                                                               const QString &customSystemPath,
                                                                               const QString &customWritablePath);
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QString libreOfficeSystemPath();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QString libreOfficeLocalPath();
Q_REQUIRED_RESULT TEXTAUTOCORRECTION_EXPORT QStringList wordsFromSentence(const QString &string);
};
}
TEXTAUTOCORRECTION_EXPORT QDebug operator<<(QDebug d, TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes t);
