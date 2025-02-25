/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelement.h"

AutoGenerateInteractionUiViewOverflowElement::AutoGenerateInteractionUiViewOverflowElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewOverflowElement::~AutoGenerateInteractionUiViewOverflowElement()
{
}

void AutoGenerateInteractionUiViewOverflowElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
}

QWidget *AutoGenerateInteractionUiViewOverflowElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}
