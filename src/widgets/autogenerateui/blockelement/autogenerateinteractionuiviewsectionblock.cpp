/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewsectionblock.h"

AutoGenerateInteractionUiViewSectionBlock::AutoGenerateInteractionUiViewSectionBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewSectionBlock::~AutoGenerateInteractionUiViewSectionBlock()
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t)
{
    // TODO implement debug t
    return d;
}

bool AutoGenerateInteractionUiViewSectionBlock::operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

void AutoGenerateInteractionUiViewSectionBlock::generateWidget(QWidget *parent)
{
    // TODO
}
