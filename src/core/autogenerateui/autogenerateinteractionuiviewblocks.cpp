/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblocks.h"
#include "blockelement/autogenerateinteractionuiviewactionsblock.h"
#include "blockelement/autogenerateinteractionuiviewdividerblock.h"
#include "blockelement/autogenerateinteractionuiviewinputblock.h"
#include "blockelement/autogenerateinteractionuiviewsectionblock.h"
#include "ruqola_action_buttons_debug.h"

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewBlocks::AutoGenerateInteractionUiViewBlocks() = default;

AutoGenerateInteractionUiViewBlocks::~AutoGenerateInteractionUiViewBlocks() = default;

void AutoGenerateInteractionUiViewBlocks::parse(const QJsonArray &array)
{
    for (const auto &r : array) {
        const QString type = r["type"_L1].toString();
        if (type == "section"_L1) {
            AutoGenerateInteractionUiViewSectionBlock section;
            section.parse(r.toObject());
            mBlockElements.append(std::move(section));
        } else if (type == "divider"_L1) {
            AutoGenerateInteractionUiViewDividerBlock divider;
            divider.parse(r.toObject());
            mBlockElements.append(std::move(divider));
        } else if (type == "actions"_L1) {
            AutoGenerateInteractionUiViewActionsBlock actions;
            actions.parse(r.toObject());
            mBlockElements.append(std::move(actions));
        } else if (type == "input"_L1) {
            AutoGenerateInteractionUiViewInputBlock input;
            input.parse(r.toObject());
            mBlockElements.append(std::move(input));
        } else if (type == "context"_L1) {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Not implemented yet:" << type;
            // TODO
        } else if (type == "image"_L1) {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Not implemented yet:" << type;
            // TODO
        } else if (type == "callout"_L1) {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Not implemented yet:" << type;
            // TODO
        } else {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown type " << type;
        }
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t)
{
    d.space() << "blockElements:" << t.blockElements();
    return d;
}

bool AutoGenerateInteractionUiViewBlocks::operator==(const AutoGenerateInteractionUiViewBlocks &other) const
{
    // TODO
    return other.mBlockElements == mBlockElements;
}

QList<AutoGenerateInteractionUiViewBlockBase> AutoGenerateInteractionUiViewBlocks::blockElements() const
{
    return mBlockElements;
}

void AutoGenerateInteractionUiViewBlocks::setBlockElements(const QList<AutoGenerateInteractionUiViewBlockBase> &newBlockElements)
{
    mBlockElements = newBlockElements;
}
