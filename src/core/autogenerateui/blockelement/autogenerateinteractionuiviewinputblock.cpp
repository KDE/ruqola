/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblock.h"

AutoGenerateInteractionUiViewInputBlock::AutoGenerateInteractionUiViewInputBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewInputBlock::~AutoGenerateInteractionUiViewInputBlock()
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t)
{
    // TODO implement debug t
    return d;
}

bool AutoGenerateInteractionUiViewInputBlock::operator==(const AutoGenerateInteractionUiViewInputBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}
