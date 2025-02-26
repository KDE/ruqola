/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewcalloutblock.h"

AutoGenerateInteractionUiViewCalloutBlock::AutoGenerateInteractionUiViewCalloutBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewCalloutBlock::~AutoGenerateInteractionUiViewCalloutBlock() = default;

QWidget *AutoGenerateInteractionUiViewCalloutBlock::generateWidget(QWidget *parent) const
{
    return nullptr;
}

void AutoGenerateInteractionUiViewCalloutBlock::parse(const QJsonObject &json)
{
    // TODO
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t)
{
    // TODO
    return d;
}
