/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QHash>
#include <QSet>
#include <QString>

#include "autocorrection/autocorrectionutils.h"

#include "pimcommonautocorrection_private_export.h"
namespace PimCommonAutoCorrection
{
class PIMCOMMONAUTOCORRECTION_TESTS_EXPORT ImportAbstractAutocorrection
{
    Q_GADGET
public:
    enum LoadAttribute {
        All = 0,
        SuperScript,
    };
    Q_ENUM(LoadAttribute)

    ImportAbstractAutocorrection();
    virtual ~ImportAbstractAutocorrection();

    virtual bool import(const QString &fileName, QString &errorMessage, ImportAbstractAutocorrection::LoadAttribute loadAttribute = All) = 0;

    Q_REQUIRED_RESULT QSet<QString> upperCaseExceptions() const;
    Q_REQUIRED_RESULT QSet<QString> twoUpperLetterExceptions() const;
    Q_REQUIRED_RESULT QHash<QString, QString> autocorrectEntries() const;
    Q_REQUIRED_RESULT QHash<QString, QString> superScriptEntries() const;

    Q_REQUIRED_RESULT AutoCorrectionUtils::TypographicQuotes typographicSingleQuotes() const;
    Q_REQUIRED_RESULT AutoCorrectionUtils::TypographicQuotes typographicDoubleQuotes() const;

    Q_REQUIRED_RESULT int maxFindStringLenght() const;

    Q_REQUIRED_RESULT int minFindStringLenght() const;

protected:
    int mMaxFindStringLength = 0;
    int mMinFindStringLength = 0;
    QSet<QString> mUpperCaseExceptions;
    QSet<QString> mTwoUpperLetterExceptions;
    QHash<QString, QString> mAutocorrectEntries;
    QHash<QString, QString> mSuperScriptEntries;
    AutoCorrectionUtils::TypographicQuotes mTypographicSingleQuotes;
    AutoCorrectionUtils::TypographicQuotes mTypographicDoubleQuotes;
};
}
