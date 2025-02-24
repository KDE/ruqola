/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionsblock.h"

AutoGenerateInteractionUiViewActionsBlock::AutoGenerateInteractionUiViewActionsBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewActionsBlock::~AutoGenerateInteractionUiViewActionsBlock()
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t)
{
    // TODO implement debug t
    return d;
}

bool AutoGenerateInteractionUiViewActionsBlock::operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}
