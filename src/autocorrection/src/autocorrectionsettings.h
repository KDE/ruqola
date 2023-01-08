/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "autocorrectionutils.h"
#include "textautocorrection_export.h"
#include <QDebug>
#include <QSet>
namespace TextAutoCorrection
{
class AutoCorrectionSettingsPrivate;
class TEXTAUTOCORRECTION_EXPORT AutoCorrectionSettings
{
public:
    AutoCorrectionSettings();
    ~AutoCorrectionSettings();

    Q_REQUIRED_RESULT bool isEnabledAutoCorrection() const;
    Q_REQUIRED_RESULT bool isUppercaseFirstCharOfSentence() const;
    Q_REQUIRED_RESULT bool isFixTwoUppercaseChars() const;
    Q_REQUIRED_RESULT bool isSingleSpaces() const;
    Q_REQUIRED_RESULT bool isAutoFractions() const;
    Q_REQUIRED_RESULT bool isCapitalizeWeekDays() const;
    Q_REQUIRED_RESULT bool isReplaceDoubleQuotes() const;
    Q_REQUIRED_RESULT bool isReplaceSingleQuotes() const;
    Q_REQUIRED_RESULT bool isAdvancedAutocorrect() const;
    Q_REQUIRED_RESULT bool isAutoFormatUrl() const;
    Q_REQUIRED_RESULT bool isAutoBoldUnderline() const;
    Q_REQUIRED_RESULT bool isSuperScript() const;

    Q_REQUIRED_RESULT bool isAddNonBreakingSpace() const;
    Q_REQUIRED_RESULT bool isReplaceDoubleQuotesByFrenchQuotes() const;

    Q_REQUIRED_RESULT TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes typographicSingleQuotes() const;
    Q_REQUIRED_RESULT TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes typographicDoubleQuotes() const;
    void setTypographicSingleQuotes(TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes singleQuote);
    void setTypographicDoubleQuotes(TextAutoCorrection::AutoCorrectionUtils::TypographicQuotes doubleQuote);

    void setAutoFormatUrl(bool b);
    void setAutoBoldUnderline(bool b);
    void setSuperScript(bool b);
    void setAddNonBreakingSpace(bool b);
    void setCapitalizeWeekDays(bool b);
    void setReplaceDoubleQuotes(bool b);
    void setReplaceSingleQuotes(bool b);
    void setAdvancedAutocorrect(bool b);
    void setEnabledAutoCorrection(bool b);
    void setReplaceDoubleQuotesByFrenchQuotes(bool b);

    void readConfig();

    void writeConfig();

    void setAutoFractions(bool newAutoFractions);

    void setSingleSpaces(bool newSingleSpaces);

    void setFixTwoUppercaseChars(bool newFixTwoUppercaseChars);

    void setUppercaseFirstCharOfSentence(bool newUppercaseFirstCharOfSentence);

    void setUpperCaseExceptions(const QSet<QString> &exceptions);
    void setTwoUpperLetterExceptions(const QSet<QString> &exceptions);
    Q_REQUIRED_RESULT QSet<QString> upperCaseExceptions() const;
    Q_REQUIRED_RESULT QSet<QString> twoUpperLetterExceptions() const;

    void setLanguage(const QString &lang, bool forceGlobal = false);
    Q_REQUIRED_RESULT QString language() const;

    Q_REQUIRED_RESULT bool isFrenchLanguage() const;
    void setAutocorrectEntries(const QHash<QString, QString> &entries);

    Q_REQUIRED_RESULT QHash<QString, QString> autocorrectEntries() const;
    Q_REQUIRED_RESULT bool addAutoCorrect(const QString &currentWord, const QString &replaceWord);

    Q_REQUIRED_RESULT QChar nonBreakingSpace() const;
    void setNonBreakingSpace(const QChar &newNonBreakingSpace);

    Q_REQUIRED_RESULT QHash<QString, QString> superScriptEntries() const;
    void setSuperScriptEntries(const QHash<QString, QString> &newSuperScriptEntries);

    void writeAutoCorrectionFile(const QString &filename = QString());
    Q_REQUIRED_RESULT int maxFindStringLength() const;

    Q_REQUIRED_RESULT int minFindStringLength() const;

    void loadLocalFileName(const QString &localFileName, const QString &fname);
    void loadGlobalFileName(const QString &fname);

    Q_REQUIRED_RESULT AutoCorrectionUtils::TypographicQuotes doubleFrenchQuotes() const;
    void setDoubleFrenchQuotes(const AutoCorrectionUtils::TypographicQuotes &newDoubleFrenchQuotes);

    Q_REQUIRED_RESULT QString customWritablePath() const;
    void setCustomWritablePath(const QString &path);

    Q_REQUIRED_RESULT QString customSystemPath() const;
    void setCustomSystemPath(const QString &path);

private:
    void migrateKMailXmlFile();
    void readAutoCorrectionFile(bool forceGlobal = false);
    Q_REQUIRED_RESULT QString containsAutoCorrectionFile(const QString &fileName);

    friend class AutoCorrectionSettingsPrivate;
    std::unique_ptr<AutoCorrectionSettingsPrivate> const d;
};
}
TEXTAUTOCORRECTION_EXPORT QDebug operator<<(QDebug d, const TextAutoCorrection::AutoCorrectionSettings &t);
