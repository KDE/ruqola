/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewsectionblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewimageelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "ruqola_autogenerateui_debug.h"
#include <QLabel>
#include <QLayout>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewSectionBlock::AutoGenerateInteractionUiViewSectionBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewSectionBlock::~AutoGenerateInteractionUiViewSectionBlock()
{
    delete mText;
    delete mAccessory;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewSectionBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    if (t.text()) {
        d.space() << "text:" << *t.text();
    }
    if (t.accessory()) {
        d.space() << "accessory:" << *t.accessory();
    }
    return d;
}

bool AutoGenerateInteractionUiViewSectionBlock::operator==(const AutoGenerateInteractionUiViewSectionBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.text() == text() && other.accessory() == other.accessory();
}

QWidget *AutoGenerateInteractionUiViewSectionBlock::generateWidget(RocketChatAccount *account, QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);

    if (mText) {
        auto label = new QLabel(parent);
        label->setWordWrap(true);
        label->setOpenExternalLinks(true);
        label->setText(mText->generateText());
        hboxLayout->addWidget(label);
    }
    if (mAccessory) {
        auto w = mAccessory->generateWidget(parent);
        hboxLayout->addWidget(w);
    }
    return widget;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewSectionBlock::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewSectionBlock::setText(AutoGenerateInteractionUiViewText *newText)
{
    mText = newText;
}

void AutoGenerateInteractionUiViewSectionBlock::parseBlock(const QJsonObject &json)
{
    if (json.contains("text"_L1)) {
        mText = new AutoGenerateInteractionUiViewText;
        mText->parse(json["text"_L1].toObject());
    }
    if (json.contains("accessory"_L1)) {
        const QJsonObject accessoryObj = json["accessory"_L1].toObject();
        const QString type = accessoryObj["type"_L1].toString();
        if (type == "button"_L1) {
            AutoGenerateInteractionUiViewButtonElement *b = new AutoGenerateInteractionUiViewButtonElement;
            b->parse(accessoryObj);
            mAccessory = b;
        } else if (type == "datepicker"_L1) {
            AutoGenerateInteractionUiViewDatePickerElement *datePicker = new AutoGenerateInteractionUiViewDatePickerElement;
            datePicker->parse(accessoryObj);
            mAccessory = datePicker;
        } else if (type == "image"_L1) {
            AutoGenerateInteractionUiViewImageElement *img = new AutoGenerateInteractionUiViewImageElement;
            img->parse(accessoryObj);
            mAccessory = img;
        } else if (type == "overflow"_L1) {
            AutoGenerateInteractionUiViewOverflowElement *flow = new AutoGenerateInteractionUiViewOverflowElement;
            flow->parse(accessoryObj);
            mAccessory = flow;
        } else if (type == "multi_static_select"_L1) {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock not implemented yet " << type;
        } else if (type == "static_select"_L1) {
            AutoGenerateInteractionUiViewStaticSelectElement *select = new AutoGenerateInteractionUiViewStaticSelectElement;
            select->parse(accessoryObj);
            mAccessory = select;
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
        }
    }
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewSectionBlock::accessory() const
{
    return mAccessory;
}

void AutoGenerateInteractionUiViewSectionBlock::setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory)
{
    mAccessory = newAccessory;
}

void AutoGenerateInteractionUiViewSectionBlock::serializeBlock(QJsonObject &o) const
{
    if (mText) {
        o["text"_L1] = mText->serialize();
        // TODO
    }
    if (mAccessory) {
        o["accessory"_L1] = mAccessory->serialize();
    }
}
