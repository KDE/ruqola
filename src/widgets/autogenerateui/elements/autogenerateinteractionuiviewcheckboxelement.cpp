/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewcheckboxelement.h"
#include "ruqola_action_buttons_debug.h"

#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewCheckboxElement::AutoGenerateInteractionUiViewCheckboxElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewCheckboxElement::~AutoGenerateInteractionUiViewCheckboxElement()
{
    qDeleteAll(mOptions);
    delete mInitialOption;
}

void AutoGenerateInteractionUiViewCheckboxElement::parseElement(const QJsonObject &json)
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

AutoGenerateInteractionUiViewOption *AutoGenerateInteractionUiViewCheckboxElement::initialOption() const
{
    return mInitialOption;
}

void AutoGenerateInteractionUiViewCheckboxElement::setInitialOption(AutoGenerateInteractionUiViewOption *newInitialOption)
{
    mInitialOption = newInitialOption;
}

QWidget *AutoGenerateInteractionUiViewCheckboxElement::generateWidget(QWidget *parent)
{
    // TODO QCheckBox
    Q_UNUSED(parent)
    // TODO
    return nullptr;
}

bool AutoGenerateInteractionUiViewCheckboxElement::operator==(const AutoGenerateInteractionUiViewCheckboxElement &other) const
{
    // TODO mInitialOption
    return other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewCheckboxElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewCheckboxElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

void AutoGenerateInteractionUiViewCheckboxElement::serializeElement(QJsonObject &o) const
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

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    if (t.initialOption()) {
        d.space() << "initialOption:" << *t.initialOption();
    }
    return d;
}

QString AutoGenerateInteractionUiViewCheckboxElement::currentValue() const
{
    return {};
}

#include "moc_autogenerateinteractionuiviewcheckboxelement.cpp"
