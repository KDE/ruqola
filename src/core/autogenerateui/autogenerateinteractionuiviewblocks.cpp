/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblocks.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewBlocks::AutoGenerateInteractionUiViewBlocks() = default;

AutoGenerateInteractionUiViewBlocks::~AutoGenerateInteractionUiViewBlocks() = default;

void AutoGenerateInteractionUiViewBlocks::parse(const QJsonArray &array)
{
    for (const auto &r : array) {
        const QString type = r["type"_L1].toString();
        if (type == "section"_L1) {
        } else if (type == "divider"_L1) {
        } else if (type == "actions"_L1) {
        } else if (type == "context"_L1) {
        } else if (type == "image"_L1) {
        } else if (type == "callout"_L1) {
        } else if (type == "input"_L1) {
        } else {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown type " << type;
        }
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
