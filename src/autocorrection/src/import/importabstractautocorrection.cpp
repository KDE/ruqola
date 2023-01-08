/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importabstractautocorrection.h"

using namespace TextAutoCorrection;

ImportAbstractAutocorrection::ImportAbstractAutocorrection() = default;

ImportAbstractAutocorrection::~ImportAbstractAutocorrection() = default;

QSet<QString> ImportAbstractAutocorrection::upperCaseExceptions() const
{
    return mUpperCaseExceptions;
}

QSet<QString> ImportAbstractAutocorrection::twoUpperLetterExceptions() const
{
    return mTwoUpperLetterExceptions;
}

QHash<QString, QString> ImportAbstractAutocorrection::autocorrectEntries() const
{
    return mAutocorrectEntries;
}

AutoCorrectionUtils::TypographicQuotes ImportAbstractAutocorrection::typographicSingleQuotes() const
{
    return mTypographicSingleQuotes;
}

AutoCorrectionUtils::TypographicQuotes ImportAbstractAutocorrection::typographicDoubleQuotes() const
{
    return mTypographicDoubleQuotes;
}

int ImportAbstractAutocorrection::maxFindStringLenght() const
{
    return mMaxFindStringLength;
}

int ImportAbstractAutocorrection::minFindStringLenght() const
{
    return mMinFindStringLength;
}

QHash<QString, QString> ImportAbstractAutocorrection::superScriptEntries() const
{
    return mSuperScriptEntries;
}
