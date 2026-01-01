/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtoggleswitchelement.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewToggleSwitchElement::AutoGenerateInteractionUiViewToggleSwitchElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewToggleSwitchElement::~AutoGenerateInteractionUiViewToggleSwitchElement()
{
    qDeleteAll(mOptions);
    qDeleteAll(mInitialOptions);
}

void AutoGenerateInteractionUiViewToggleSwitchElement::parseElement(const QJsonObject &json)
{
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
    // initialOptions
    const QJsonArray initialOptionsArray = json["initialOptions"_L1].toArray();
    for (const auto &opt : initialOptionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mInitialOptions.append(option);
    }
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewToggleSwitchElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::serializeElement(QJsonObject &o) const
{
    QJsonArray options;
    for (const auto &r : std::as_const(mOptions)) {
        options.append(r->serialize());
    }
    o["options"_L1] = options;
    if (!mInitialOptions.isEmpty()) {
        for (const auto &r : std::as_const(mInitialOptions)) {
            options.append(r->serialize());
        }
        o["initialOptions"_L1] = options;
    }
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewToggleSwitchElement::initialOptions() const
{
    return mInitialOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOptions)
{
    mInitialOptions = newInitialOptions;
}

QVariant AutoGenerateInteractionUiViewToggleSwitchElement::currentValue() const
{
    qWarning() << " not implemented yet";
    // TODO
    return {};
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setCurrentValue(const QVariant &v)
{
    qWarning() << " not implemented yet" << v;
}

ActionElementWidget *AutoGenerateInteractionUiViewToggleSwitchElement::generateWidget(QWidget *parent)
{
    // TODO use QRadioButton ?
    qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Not implemented AutoGenerateInteractionUiViewToggleSwitchElement UI";

    Q_UNUSED(parent)
    return nullptr;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    d.space() << "initialOptions:" << t.initialOptions();
    return d;
}

bool AutoGenerateInteractionUiViewToggleSwitchElement::operator==(const AutoGenerateInteractionUiViewToggleSwitchElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.options() == options() && other.initialOptions() == initialOptions();
}

#include "moc_autogenerateinteractionuiviewtoggleswitchelement.cpp"
