/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionsblock.h"
#include "autogenerateui/autogenerateinteractionuiviewbuttonelement.h"
#include "ruqola_action_buttons_debug.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionsBlock::AutoGenerateInteractionUiViewActionsBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewActionsBlock::~AutoGenerateInteractionUiViewActionsBlock()
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t)
{
    d << "elements:" << t.elements();
    return d;
}

bool AutoGenerateInteractionUiViewActionsBlock::operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

void AutoGenerateInteractionUiViewActionsBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    for (const auto &r : json["elements"_L1].toArray()) {
        const QString type = r["type"_L1].toString();
        if (type == "button"_L1) {
            AutoGenerateInteractionUiViewButtonElement e;
            e.parse(r["text"_L1].toObject());
            mElements.append(std::move(e));
            // TODO
        } else {
            qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown type " << type;
        }
        // TODO
    }
    // TODO elements
}

QList<AutoGenerateInteractionUiViewActionable> AutoGenerateInteractionUiViewActionsBlock::elements() const
{
    return mElements;
}

void AutoGenerateInteractionUiViewActionsBlock::setElements(const QList<AutoGenerateInteractionUiViewActionable> &newElements)
{
    mElements = newElements;
}
