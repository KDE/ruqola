/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblocks.h"

AutoGenerateInteractionUiViewBlocks::AutoGenerateInteractionUiViewBlocks() = default;

AutoGenerateInteractionUiViewBlocks::~AutoGenerateInteractionUiViewBlocks() = default;

void AutoGenerateInteractionUiViewBlocks::parse(const QJsonArray &array)
{
    for (const auto &r : array) {
        // TODO parse type
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t)
{
    // TODO
    return d;
}

bool AutoGenerateInteractionUiViewBlocks::operator==(const AutoGenerateInteractionUiViewBlocks &other) const
{
    // TODO
    return false;
}
