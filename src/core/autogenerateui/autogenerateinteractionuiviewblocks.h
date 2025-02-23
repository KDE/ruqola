/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QJsonArray>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewBlocks
{
public:
    AutoGenerateInteractionUiViewBlocks();
    ~AutoGenerateInteractionUiViewBlocks();
    void parse(const QJsonArray &array);
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewBlocks &other) const;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewBlocks)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewBlocks, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t);
