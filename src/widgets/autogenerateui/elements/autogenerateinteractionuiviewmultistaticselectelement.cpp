/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewmultistaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include "autogenerateui/widgets/multistaticselectwidget.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewMultiStaticSelectElement::AutoGenerateInteractionUiViewMultiStaticSelectElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewMultiStaticSelectElement::~AutoGenerateInteractionUiViewMultiStaticSelectElement()
{
    delete mPlaceHolder;
    qDeleteAll(mOptions);
    qDeleteAll(mInitialOptions);
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::parseElement(const QJsonObject &json)
{
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
    mMaxSelectItems = json["maxSelectItems"_L1].toInt(-1);
    // TODO verify it
    const QJsonArray array = json["initialValue"_L1].toArray();
    for (int i = 0; i < array.count(); ++i) {
        mInitialValue.append(array.at(i).toString());
    }

    // initialOptions
    const QJsonArray initialOptionsArray = json["initialOptions"_L1].toArray();
    for (const auto &opt : initialOptionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mInitialOptions.append(option);
    }
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewMultiStaticSelectElement::initialOptions() const
{
    return mInitialOptions;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setInitialOptions(const QList<AutoGenerateInteractionUiViewOption *> &newInitialOptions)
{
    mInitialOptions = newInitialOptions;
}

int AutoGenerateInteractionUiViewMultiStaticSelectElement::maxSelectItems() const
{
    return mMaxSelectItems;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setMaxSelectItems(int newMaxSelectItems)
{
    mMaxSelectItems = newMaxSelectItems;
}

QVariant AutoGenerateInteractionUiViewMultiStaticSelectElement::currentValue() const
{
    return mMultiStaticSelectWidget->selectedUsers();
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setCurrentValue(const QVariant &v)
{
    // TODO
    mMultiStaticSelectWidget->setInitialValues(v.toStringList());
}

QStringList AutoGenerateInteractionUiViewMultiStaticSelectElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setInitialValue(const QStringList &newInitialValue)
{
    mInitialValue = newInitialValue;
}

ActionElementWidget *AutoGenerateInteractionUiViewMultiStaticSelectElement::generateWidget(QWidget *parent)
{
    mMultiStaticSelectWidget = new MultiStaticSelectWidget(parent);
    mActionElementWidget = new ActionElementWidget(mMultiStaticSelectWidget, actionId(), parent);
    if (mPlaceHolder) {
        mMultiStaticSelectWidget->setPlaceholderText(mPlaceHolder->generateText());
    }
    QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> lst;
    lst.reserve(mOptions.count());
    for (const auto &opt : std::as_const(mOptions)) {
        const MultiStaticSelectLineEditModel::SelectItemCompletionInfo info{
            .text = opt->text().generateText(true),
            .value = opt->value(),
        };
        lst.append(std::move(info));
    }
    mMultiStaticSelectWidget->setUserCompletionInfos(lst);
    if (mMaxSelectItems != -1) {
        mMultiStaticSelectWidget->setMaxSelectItems(mMaxSelectItems);
    }
    mMultiStaticSelectWidget->setInitialValues(mInitialValue);
    // TODO use mInitialOptions
    return mActionElementWidget;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewMultiStaticSelectElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewMultiStaticSelectElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    d.space() << "options:" << t.options();
    d.space() << "initialValue:" << t.initialValue();
    d.space() << "maxSelectItems:" << t.maxSelectItems();
    d.space() << "initialOptions:" << t.initialOptions();
    return d;
}

bool AutoGenerateInteractionUiViewMultiStaticSelectElement::operator==(const AutoGenerateInteractionUiViewMultiStaticSelectElement &other) const
{
    // TODO add placeholder too
    return other.maxSelectItems() == maxSelectItems() && other.initialValue() == initialValue() && other.options() == options()
        && other.initialOptions() == initialOptions() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::serializeElement(QJsonObject &o) const
{
    if (mPlaceHolder) {
        o["placeholder"_L1] = mPlaceHolder->serialize();
    }
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
    if (mMaxSelectItems != -1) {
        o["maxSelectItems"_L1] = mMaxSelectItems;
    }
}

#include "moc_autogenerateinteractionuiviewmultistaticselectelement.cpp"
