/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewdividerblock.h"

AutoGenerateInteractionUiViewDividerBlock::AutoGenerateInteractionUiViewDividerBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewDividerBlock::~AutoGenerateInteractionUiViewDividerBlock()
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t)
{
    // TODO implement debug t
    return d;
}

bool AutoGenerateInteractionUiViewDividerBlock::operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

QWidget *AutoGenerateInteractionUiViewDividerBlock::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}
