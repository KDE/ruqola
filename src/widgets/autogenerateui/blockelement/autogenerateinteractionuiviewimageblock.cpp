/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewimageblock.h"

AutoGenerateInteractionUiViewImageBlock::AutoGenerateInteractionUiViewImageBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewImageBlock::~AutoGenerateInteractionUiViewImageBlock() = default;

QWidget *AutoGenerateInteractionUiViewImageBlock::generateWidget(QWidget *parent) const
{
    return nullptr;
}

void AutoGenerateInteractionUiViewImageBlock::parse(const QJsonObject &json)
{
    // TODO
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t)
{
    // TODO
    return d;
}
