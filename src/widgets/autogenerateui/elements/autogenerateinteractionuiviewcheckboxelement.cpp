/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewcheckboxelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include <QCheckBox>
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewCheckboxElement::AutoGenerateInteractionUiViewCheckboxElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewCheckboxElement::~AutoGenerateInteractionUiViewCheckboxElement()
{
    qDeleteAll(mOptions);
    qDeleteAll(mInitialOptions);
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
    const QJsonArray initialOptionsArray = json["initialOption"_L1].toArray();
    for (const auto &opt : initialOptionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mInitialOptions.append(option);
    }
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewCheckboxElement::initialOptions() const
{
    return mInitialOptions;
}

void AutoGenerateInteractionUiViewCheckboxElement::setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOption)
{
    mInitialOptions = newInitialOption;
}

ActionElementWidget *AutoGenerateInteractionUiViewCheckboxElement::generateWidget(QWidget *parent)
{
    // TODO list of checkbox ????
    mCheckBox = new QCheckBox(parent);
    mActionElementWidget = new ActionElementWidget(mCheckBox, actionId(), parent);
    return mActionElementWidget;
}

bool AutoGenerateInteractionUiViewCheckboxElement::operator==(const AutoGenerateInteractionUiViewCheckboxElement &other) const
{
    return other.initialOptions() == initialOptions() && other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
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
    if (!mInitialOptions.isEmpty()) {
        QJsonArray initialOptions;
        for (const auto &r : std::as_const(mInitialOptions)) {
            initialOptions.append(r->serialize());
        }
        o["initialOption"_L1] = initialOptions;
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    d.space() << "initialOption:" << t.initialOptions();
    return d;
}

QVariant AutoGenerateInteractionUiViewCheckboxElement::currentValue() const
{
    return mCheckBox->isChecked();
}

void AutoGenerateInteractionUiViewCheckboxElement::setCurrentValue(const QVariant &v)
{
    mCheckBox->setCheckable(v.toBool());
}

#include "moc_autogenerateinteractionuiviewcheckboxelement.cpp"
