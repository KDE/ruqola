/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewdividerblock.h"

#include <KSeparator>

AutoGenerateInteractionUiViewDividerBlock::AutoGenerateInteractionUiViewDividerBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewDividerBlock::~AutoGenerateInteractionUiViewDividerBlock() = default;

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    return d;
}

bool AutoGenerateInteractionUiViewDividerBlock::operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

QWidget *AutoGenerateInteractionUiViewDividerBlock::generateWidget(QWidget *parent) const
{
    return new KSeparator(parent);
}

QJsonObject AutoGenerateInteractionUiViewDividerBlock::serialize() const
{
    return {};
}
