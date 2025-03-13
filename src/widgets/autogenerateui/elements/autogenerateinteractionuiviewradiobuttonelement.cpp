/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewradiobuttonelement.h"
#include "ruqola_action_buttons_debug.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewRadioButtonElement::AutoGenerateInteractionUiViewRadioButtonElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewRadioButtonElement::~AutoGenerateInteractionUiViewRadioButtonElement()
{
    qDeleteAll(mOptions);
    delete mInitialOption;
}

void AutoGenerateInteractionUiViewRadioButtonElement::parseElement(const QJsonObject &json)
{
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
    if (json.contains("initialOption"_L1)) {
        mInitialOption = new AutoGenerateInteractionUiViewOption;
        mInitialOption->parse(json["initialOption"_L1].toObject());
    }
}

AutoGenerateInteractionUiViewOption *AutoGenerateInteractionUiViewRadioButtonElement::initialOption() const
{
    return mInitialOption;
}

void AutoGenerateInteractionUiViewRadioButtonElement::setInitialOption(AutoGenerateInteractionUiViewOption *newInitialOption)
{
    mInitialOption = newInitialOption;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewRadioButtonElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewRadioButtonElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QWidget *AutoGenerateInteractionUiViewRadioButtonElement::generateWidget(QWidget *parent)
{
    // QRadioButton ?
    Q_UNUSED(parent)
    return nullptr;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewRadioButtonElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    if (t.initialOption()) {
        d.space() << "initialOption:" << *t.initialOption();
    }
    return d;
}

bool AutoGenerateInteractionUiViewRadioButtonElement::operator==(const AutoGenerateInteractionUiViewRadioButtonElement &other) const
{
    // TODO initialOption ???
    return AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewRadioButtonElement::serializeElement(QJsonObject &o) const
{
    QJsonArray options;
    for (const auto &r : std::as_const(mOptions)) {
        options.append(r->serialize());
    }
    o["options"_L1] = options;
    if (mInitialOption) {
        o["initialOption"_L1] = mInitialOption->serialize();
    }
}

#include "moc_autogenerateinteractionuiviewradiobuttonelement.cpp"
