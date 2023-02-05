/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#pragma once

class LIBRUQOLACORE_EXPORT LocalGlobalDatabase : public LocalDatabaseBase
{
public:
    LocalGlobalDatabase();
    ~LocalGlobalDatabase() override;

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
