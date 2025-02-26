/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelement.h"

AutoGenerateInteractionUiViewOverflowElement::AutoGenerateInteractionUiViewOverflowElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewOverflowElement::~AutoGenerateInteractionUiViewOverflowElement()
{
}

void AutoGenerateInteractionUiViewOverflowElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
}

QWidget *AutoGenerateInteractionUiViewOverflowElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewOverflowElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewOverflowElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOverflowElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "options:" << t.options();
    return d;
}

bool AutoGenerateInteractionUiViewOverflowElement::operator==(const AutoGenerateInteractionUiViewOverflowElement &other) const
{
    return other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
}
