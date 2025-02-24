/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewSectionBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewSectionBlock();
    ~AutoGenerateInteractionUiViewSectionBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const;
    void generateWidget(QWidget *parent) override;

private:
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewSectionBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewSectionBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_NO_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t);
