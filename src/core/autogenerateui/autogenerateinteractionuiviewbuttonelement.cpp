/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewbuttonelement.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewButtonElement::AutoGenerateInteractionUiViewButtonElement() = default;
AutoGenerateInteractionUiViewButtonElement::~AutoGenerateInteractionUiViewButtonElement() = default;

bool AutoGenerateInteractionUiViewButtonElement::isValid() const
{
    // TODO
    return false;
}

AutoGenerateInteractionUiViewButtonElement::Style AutoGenerateInteractionUiViewButtonElement::convertStyleFromString(const QString &str) const
{
    if (str == "primary"_L1) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Primary;
    } else if (str == "secondary"_L1) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Secondary;
    } else if (str == "danger"_L1) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Danger;
    } else if (str == "warning"_L1) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Warning;
    } else if (str == "success"_L1) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Success;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "Unknown Style type " << str;
    }
    return AutoGenerateInteractionUiViewButtonElement::Style::Unknown;
}

AutoGenerateInteractionUiViewButtonElement::Style AutoGenerateInteractionUiViewButtonElement::style() const
{
    return mStyle;
}

void AutoGenerateInteractionUiViewButtonElement::setStyle(Style newStyle)
{
    mStyle = newStyle;
}

bool AutoGenerateInteractionUiViewButtonElement::operator==(const AutoGenerateInteractionUiViewButtonElement &other) const
{
    return other.style() == style();
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewButtonElement::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewButtonElement::setText(const AutoGenerateInteractionUiViewText &newText)
{
    mText = newText;
}

void AutoGenerateInteractionUiViewButtonElement::parse(const QJsonObject &json)
{
    mStyle = convertStyleFromString(json["style"_L1].toString());
    mText.parse(json["text"_L1].toObject());
    // TODO
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t)
{
    d.space() << "style:" << t.style();
    d.space() << "text:" << t.text();
    return d;
}

#include "moc_autogenerateinteractionuiviewbuttonelement.cpp"
