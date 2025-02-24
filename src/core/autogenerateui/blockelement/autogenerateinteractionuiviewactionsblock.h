/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewActionsBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewActionsBlock();
    ~AutoGenerateInteractionUiViewActionsBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const;

private:
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewActionsBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewActionsBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t);
