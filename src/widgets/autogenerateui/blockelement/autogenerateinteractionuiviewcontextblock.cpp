/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewcontextblock.h"

AutoGenerateInteractionUiViewContextBlock::AutoGenerateInteractionUiViewContextBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewContextBlock::~AutoGenerateInteractionUiViewContextBlock()
{
}

bool AutoGenerateInteractionUiViewContextBlock::operator==(const AutoGenerateInteractionUiViewContextBlock &other) const
{
    // TODO
    return false;
}

QWidget *AutoGenerateInteractionUiViewContextBlock::generateWidget(QWidget *parent) const
{
    // TODO
    return nullptr;
}

void AutoGenerateInteractionUiViewContextBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewContextBlock &t)
{
    // TODO
    return d;
}
