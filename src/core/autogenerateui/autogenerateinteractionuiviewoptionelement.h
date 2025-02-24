/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewactionable.h"

// See ./packages/ui-kit/src/blocks/Option.ts
class AutoGenerateInteractionUiViewOptionElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewOptionElement();
    ~AutoGenerateInteractionUiViewOptionElement() override;
    void parse(const QJsonObject &json) override;

private:
    QString value;
    // TODO add url/description
    // TODO add textobject
};
