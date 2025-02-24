/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewradiobuttonelement.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewRadioButtonElement::AutoGenerateInteractionUiViewRadioButtonElement() = default;
AutoGenerateInteractionUiViewRadioButtonElement::~AutoGenerateInteractionUiViewRadioButtonElement() = default;

void AutoGenerateInteractionUiViewRadioButtonElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewRadioButtonElement &t)
{
    // TODO use qDebug with AutoGenerateInteractionUiViewActionable
    return d;
}

#include "moc_autogenerateinteractionuiviewradiobuttonelement.cpp"
