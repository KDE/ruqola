/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewDividerBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewDividerBlock();
    ~AutoGenerateInteractionUiViewDividerBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewDividerBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewDividerBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t);
