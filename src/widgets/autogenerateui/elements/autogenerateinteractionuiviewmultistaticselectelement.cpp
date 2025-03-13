/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewmultistaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/widgets/multistaticselectwidget.h"

#include <QJsonArray>
#include <QLineEdit>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewMultiStaticSelectElement::AutoGenerateInteractionUiViewMultiStaticSelectElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewMultiStaticSelectElement::~AutoGenerateInteractionUiViewMultiStaticSelectElement()
{
    delete mPlaceHolder;
    qDeleteAll(mOptions);
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
}

int AutoGenerateInteractionUiViewMultiStaticSelectElement::maxSelectItems() const
{
    return mMaxSelectItems;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setMaxSelectItems(int newMaxSelectItems)
{
    mMaxSelectItems = newMaxSelectItems;
}

QStringList AutoGenerateInteractionUiViewMultiStaticSelectElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setInitialValue(const QStringList &newInitialValue)
{
    mInitialValue = newInitialValue;
}

QWidget *AutoGenerateInteractionUiViewMultiStaticSelectElement::generateWidget(QWidget *parent)
{
    auto lineEdit = new MultiStaticSelectWidget(parent);
    if (mPlaceHolder) {
        lineEdit->setPlaceholderText(mPlaceHolder->generateText());
    }
    QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> lst;
    lst.reserve(mOptions.count());
    for (const auto &opt : std::as_const(mOptions)) {
        MultiStaticSelectLineEditModel::SelectItemCompletionInfo info;
        info.text = opt->text().generateText(true);
        info.value = opt->value();
        lst.append(std::move(info));
    }
    lineEdit->setUserCompletionInfos(lst);
    if (mMaxSelectItems != -1) {
        lineEdit->setMaxSelectItems(mMaxSelectItems);
    }
    // TODO lineEdit->setText(mInitialValue);
    return lineEdit;
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
    return d;
}

bool AutoGenerateInteractionUiViewMultiStaticSelectElement::operator==(const AutoGenerateInteractionUiViewMultiStaticSelectElement &other) const
{
    // TODO add placeholder too
    return other.maxSelectItems() == maxSelectItems() && other.initialValue() == initialValue() && other.options() == options()
        && AutoGenerateInteractionUiViewActionable::operator==(other);
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
}

#include "moc_autogenerateinteractionuiviewmultistaticselectelement.cpp"
