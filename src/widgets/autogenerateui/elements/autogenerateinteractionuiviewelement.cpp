/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewelement.h"
// See ./packages/ui-kit/src/surfaces/modal/UiKitParserModal.spec.ts

AutoGenerateInteractionUiViewElement::AutoGenerateInteractionUiViewElement() = default;

AutoGenerateInteractionUiViewElement::~AutoGenerateInteractionUiViewElement() = default;

void AutoGenerateInteractionUiViewElement::parse(const QJsonObject &json)
{
    Q_UNUSED(json);
    // Implement in sub class
}
