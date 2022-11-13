/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportabstractautocorrection.h"
using namespace PimCommonAutoCorrection;

ExportAbstractAutocorrection::ExportAbstractAutocorrection() = default;

ExportAbstractAutocorrection::~ExportAbstractAutocorrection() = default;

void ExportAbstractAutocorrection::setUpperCaseExceptions(const QSet<QString> &newUpperCaseExceptions)
{
    mUpperCaseExceptions = newUpperCaseExceptions;
}

void ExportAbstractAutocorrection::setTwoUpperLetterExceptions(const QSet<QString> &newTwoUpperLetterExceptions)
{
    mTwoUpperLetterExceptions = newTwoUpperLetterExceptions;
}

void ExportAbstractAutocorrection::setAutocorrectEntries(const QHash<QString, QString> &newAutocorrectEntries)
{
    mAutocorrectEntries = newAutocorrectEntries;
}
