/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewbuttonelement.h"
#include "ruqola_autogenerateui_debug.h"
#include <QPushButton>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewButtonElement::AutoGenerateInteractionUiViewButtonElement() = default;
AutoGenerateInteractionUiViewButtonElement::~AutoGenerateInteractionUiViewButtonElement() = default;

AutoGenerateInteractionUiViewButtonElement::Style AutoGenerateInteractionUiViewButtonElement::convertStyleFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return AutoGenerateInteractionUiViewButtonElement::Style::Unknown;
    } else if (str == "primary"_L1) {
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
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown Style type " << str;
    }
    return AutoGenerateInteractionUiViewButtonElement::Style::Unknown;
}

bool AutoGenerateInteractionUiViewButtonElement::secondary() const
{
    return mSecondary;
}

void AutoGenerateInteractionUiViewButtonElement::setSecondary(bool newSecondary)
{
    mSecondary = newSecondary;
}

QWidget *AutoGenerateInteractionUiViewButtonElement::generateWidget(QWidget *parent)
{
    auto b = new QPushButton(parent);
    b->setText(mText.text());
    // TODO add Style
    return b;
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
    return other.style() == style() && other.text() == text() && other.secondary() == secondary();
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
    qDebug() << " AutoGenerateInteractionUiViewButtonElement::parse " << json;
    AutoGenerateInteractionUiViewActionable::parse(json);
    mStyle = convertStyleFromString(json["style"_L1].toString());
    mText.parse(json["text"_L1].toObject());
    mSecondary = json["secondary"_L1].toBool();
    // TODO
    qDebug() << "AutoGenerateInteractionUiViewButtonElement *this " << *this;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t)
{
    d.space() << "style:" << t.style();
    d.space() << "text:" << t.text();
    d.space() << "secondary:" << t.secondary();
    // TODO use qDebug with AutoGenerateInteractionUiViewActionable
    return d;
}

#include "moc_autogenerateinteractionuiviewbuttonelement.cpp"
