/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importabstractautocorrection.h"
#include "textautocorrection_private_export.h"
namespace TextAutoCorrection
{
class TEXTAUTOCORRECTION_TESTS_EXPORT ImportKMailAutocorrection : public ImportAbstractAutocorrection
{
public:
    ImportKMailAutocorrection();
    ~ImportKMailAutocorrection() override;
    Q_REQUIRED_RESULT bool import(const QString &fileName, QString &errorMessage, ImportAbstractAutocorrection::LoadAttribute loadAttribute = All) override;
};
}
