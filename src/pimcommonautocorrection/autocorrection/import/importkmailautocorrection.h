/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "importabstractautocorrection.h"
#include "pimcommonautocorrection_private_export.h"
namespace PimCommonAutoCorrection
{
class PIMCOMMONAUTOCORRECTION_TESTS_EXPORT ImportKMailAutocorrection : public ImportAbstractAutocorrection
{
public:
    ImportKMailAutocorrection();
    ~ImportKMailAutocorrection() override;
    Q_REQUIRED_RESULT bool import(const QString &fileName, QString &errorMessage, ImportAbstractAutocorrection::LoadAttribute loadAttribute = All) override;
};
}
