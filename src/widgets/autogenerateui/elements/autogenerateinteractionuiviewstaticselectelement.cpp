/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include "ruqola_action_buttons_debug.h"
#include <QComboBox>
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewStaticSelectElement::AutoGenerateInteractionUiViewStaticSelectElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewStaticSelectElement::~AutoGenerateInteractionUiViewStaticSelectElement()
{
    delete mPlaceHolder;
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewStaticSelectElement::parseElement(const QJsonObject &json)
{
    mInitialValue = json["initialValue"_L1].toString();
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
}

QWidget *AutoGenerateInteractionUiViewStaticSelectElement::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    Q_UNUSED(account)
    auto comboBox = new QComboBox(parent);
    for (const auto &r : std::as_const(mOptions)) {
        comboBox->addItem(r->text().text(), r->value());
    }
    connect(comboBox, &QComboBox::activated, this, [this, comboBox](int index) {
        if (index != -1) {
            qDebug() << "combobox mActionId******** " << mActionId;
            Q_EMIT actionChanged(mActionId, comboBox->itemData(index).toString());
        }
    });
    return comboBox;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewStaticSelectElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QString AutoGenerateInteractionUiViewStaticSelectElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setInitialValue(const QString &newInitialValue)
{
    mInitialValue = newInitialValue;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewStaticSelectElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

void AutoGenerateInteractionUiViewStaticSelectElement::serializeElement(QJsonObject &o) const
{
    // TODO initial value ? is current value §????
    o["initialValue"_L1] = mInitialValue;
    if (mPlaceHolder) {
        o["placeholder"_L1] = mPlaceHolder->serialize();
    }
    QJsonArray options;
    for (const auto &r : std::as_const(mOptions)) {
        options.append(r->serialize());
    }
    o["options"_L1] = options;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "initialValue:" << t.initialValue();
    d.space() << "options:" << t.options();
    return d;
}

#include "moc_autogenerateinteractionuiviewstaticselectelement.cpp"
