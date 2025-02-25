/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
// ./packages/ui-kit/src/blocks/elements/OverflowElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewOverflowElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewOverflowElement();
    ~AutoGenerateInteractionUiViewOverflowElement();
    void parse(const QJsonObject &json) override;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
};
