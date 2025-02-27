/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblock.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewplaintextinputelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtimepickerelement.h"
#include "ruqola_autogenerateui_debug.h"

#include <QLabel>
#include <QLayout>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewInputBlock::AutoGenerateInteractionUiViewInputBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewInputBlock::~AutoGenerateInteractionUiViewInputBlock()
{
    delete mElement;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    d.space() << "optional:" << t.optional();
    d.space() << "label:" << t.label();
    return d;
}

bool AutoGenerateInteractionUiViewInputBlock::operator==(const AutoGenerateInteractionUiViewInputBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.optional() == optional() && other.label() == label();
}

void AutoGenerateInteractionUiViewInputBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    mOptional = json["optional"_L1].toBool();
    mLabel.parse(json["label"_L1].toObject());
    const QJsonObject elementObj = json["element"_L1].toObject();
    const QString type = elementObj["type"_L1].toString();
    if (type == "plain_text_input"_L1) {
        mElement = new AutoGenerateInteractionUiViewPlainTextInputElement;
        mElement->parse(elementObj);
    } else if (type == "datepicker"_L1) {
        mElement = new AutoGenerateInteractionUiViewDatePickerElement;
        mElement->parse(elementObj);
    } else if (type == "time_picker"_L1) {
        mElement = new AutoGenerateInteractionUiViewTimePickerElement;
        mElement->parse(elementObj);
    } else if (type == "static_select"_L1) {
        mElement = new AutoGenerateInteractionUiViewStaticSelectElement;
        mElement->parse(elementObj);
    } else {
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
    }
}

bool AutoGenerateInteractionUiViewInputBlock::optional() const
{
    return mOptional;
}

void AutoGenerateInteractionUiViewInputBlock::setOptional(bool newOptional)
{
    mOptional = newOptional;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewInputBlock::label() const
{
    return mLabel;
}

void AutoGenerateInteractionUiViewInputBlock::setLabel(const AutoGenerateInteractionUiViewText &newLabel)
{
    mLabel = newLabel;
}

QWidget *AutoGenerateInteractionUiViewInputBlock::generateWidget(QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    widget->setLayout(vboxLayout);
    auto label = new QLabel(parent);
    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);
    label->setText(mLabel.generateText());
    vboxLayout->addWidget(label);
    if (mElement) {
        vboxLayout->addWidget(mElement->generateWidget(parent));
    }
    return widget;
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewInputBlock::element() const
{
    return mElement;
}

void AutoGenerateInteractionUiViewInputBlock::setElement(AutoGenerateInteractionUiViewActionable *newElement)
{
    mElement = newElement;
}
