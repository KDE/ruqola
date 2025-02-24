/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewInputBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewInputBlock();
    ~AutoGenerateInteractionUiViewInputBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewInputBlock &other) const;

private:
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewInputBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewInputBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t);
