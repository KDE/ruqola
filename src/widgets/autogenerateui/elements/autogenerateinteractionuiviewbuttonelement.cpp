/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include "colorsandmessageviewstyle.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonObject>
#include <QPushButton>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewButtonElement::AutoGenerateInteractionUiViewButtonElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewButtonElement::~AutoGenerateInteractionUiViewButtonElement() = default;

QString AutoGenerateInteractionUiViewButtonElement::convertStyleToString() const
{
    switch (mStyle) {
    case AutoGenerateInteractionUiViewButtonElement::Style::Unknown:
        return {};
    case AutoGenerateInteractionUiViewButtonElement::Style::Primary:
        return "primary"_L1;
    case AutoGenerateInteractionUiViewButtonElement::Style::Secondary:
        return "secondary"_L1;
    case AutoGenerateInteractionUiViewButtonElement::Style::Danger:
        return "danger"_L1;
    case AutoGenerateInteractionUiViewButtonElement::Style::Warning:
        return "warning"_L1;
    case AutoGenerateInteractionUiViewButtonElement::Style::Success:
        return "success"_L1;
    }
    return {};
}

QString AutoGenerateInteractionUiViewButtonElement::value() const
{
    return mValue;
}

void AutoGenerateInteractionUiViewButtonElement::setValue(const QString &newValue)
{
    mValue = newValue;
}

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

ActionElementWidget *AutoGenerateInteractionUiViewButtonElement::generateWidget(QWidget *parent)
{
    auto b = new QPushButton(parent);
    mActionElementWidget = new ActionElementWidget(b, actionId(), parent);
    b->setText(mText.generateText(true));
    connect(b, &QPushButton::clicked, this, [this]() {
        // qDebug() << "BUTTON: mBlockId " << mBlockId << " mValue " << mValue << " mActionId " << mActionId;
        Q_EMIT actionChanged(mActionId, mValue);
    });
    switch (mStyle) {
    case Style::Unknown:
        break;
    case Style::Danger: {
        QPalette p = b->palette();
        p.setColor(QPalette::Button, ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::NegativeBackground).color());
        p.setColor(QPalette::ButtonText, ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
        b->setPalette(p);
        break;
    }
    case Style::Primary: {
        QPalette p = b->palette();
        p.setColor(QPalette::ButtonText, ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::ActiveText).color());
        p.setColor(QPalette::Button, ColorsAndMessageViewStyle::self().schemeView().background(KColorScheme::ActiveBackground).color());
        b->setPalette(p);
        break;
    }
    case Style::Secondary:
    case Style::Warning:
    case Style::Success:
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unimplemented style for " << mStyle;
        break;
    }

    return mActionElementWidget;
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
    return other.value() == value() && other.style() == style() && other.text() == text() && other.secondary() == secondary()
        && AutoGenerateInteractionUiViewActionable::operator==(other);
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewButtonElement::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewButtonElement::setText(const AutoGenerateInteractionUiViewText &newText)
{
    mText = newText;
}

void AutoGenerateInteractionUiViewButtonElement::parseElement(const QJsonObject &json)
{
    // qDebug() << " AutoGenerateInteractionUiViewButtonElement::parse " << json;
    mStyle = convertStyleFromString(json["style"_L1].toString());
    mText.parse(json["text"_L1].toObject());
    mSecondary = json["secondary"_L1].toBool();
    mValue = json["value"_L1].toString();
    // qDebug() << "AutoGenerateInteractionUiViewButtonElement *this " << *this;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "style:" << t.style();
    d.space() << "text:" << t.text();
    d.space() << "secondary:" << t.secondary();
    d.space() << "value:" << t.value();
    return d;
}

void AutoGenerateInteractionUiViewButtonElement::serializeElement(QJsonObject &o) const
{
    o["text"_L1] = mText.serialize();
    const QString style = convertStyleToString();
    if (!style.isEmpty()) {
        o["style"_L1] = style;
    }
    if (!mValue.isEmpty()) {
        o["value"_L1] = mValue;
    }
    o["secondary"_L1] = mSecondary;
}

#include "moc_autogenerateinteractionuiviewbuttonelement.cpp"
