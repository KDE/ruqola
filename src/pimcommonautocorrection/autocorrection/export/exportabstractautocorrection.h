/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QHash>
#include <QSet>
#include <QString>

#include "pimcommonautocorrection_private_export.h"
namespace PimCommonAutoCorrection
{
class PIMCOMMONAUTOCORRECTION_TESTS_EXPORT ExportAbstractAutocorrection
{
public:
    ExportAbstractAutocorrection();
    virtual ~ExportAbstractAutocorrection();
    virtual bool exportData(const QString &language, const QString &fileName, QString &errorMessage, const QString &writablePath) = 0;

    void setUpperCaseExceptions(const QSet<QString> &newUpperCaseExceptions);
    void setTwoUpperLetterExceptions(const QSet<QString> &newTwoUpperLetterExceptions);
    void setAutocorrectEntries(const QHash<QString, QString> &newAutocorrectEntries);

protected:
    QSet<QString> mUpperCaseExceptions;
    QSet<QString> mTwoUpperLetterExceptions;
    QHash<QString, QString> mAutocorrectEntries;
};
}
