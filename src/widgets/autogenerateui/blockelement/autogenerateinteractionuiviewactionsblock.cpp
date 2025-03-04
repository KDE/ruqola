/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewactionsblock.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewcheckboxelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewlinearscaleelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewradiobuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtimepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtoggleswitchelement.h"
#include "common/flowlayout.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
#include <QLayout>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewActionsBlock::AutoGenerateInteractionUiViewActionsBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewActionsBlock::~AutoGenerateInteractionUiViewActionsBlock()
{
    qDeleteAll(mElements);
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    d.space() << "elements:" << t.elements();
    return d;
}

bool AutoGenerateInteractionUiViewActionsBlock::operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

void AutoGenerateInteractionUiViewActionsBlock::parseBlock(const QJsonObject &json)
{
    /*
                | ChannelsSelectElement
                | ConversationsSelectElement
                | MultiChannelsSelectElement
                | MultiConversationsSelectElement
                | MultiUsersSelectElement
                | UsersSelectElement
*/
    for (const auto &r : json["elements"_L1].toArray()) {
        const QString type = r["type"_L1].toString();
        if (type == "button"_L1) {
            AutoGenerateInteractionUiViewButtonElement *e = new AutoGenerateInteractionUiViewButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "checkbox"_L1) {
            AutoGenerateInteractionUiViewCheckboxElement *e = new AutoGenerateInteractionUiViewCheckboxElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "radio_button"_L1) {
            AutoGenerateInteractionUiViewRadioButtonElement *e = new AutoGenerateInteractionUiViewRadioButtonElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "toggle_switch"_L1) {
            AutoGenerateInteractionUiViewToggleSwitchElement *e = new AutoGenerateInteractionUiViewToggleSwitchElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "static_select"_L1) {
            AutoGenerateInteractionUiViewStaticSelectElement *e = new AutoGenerateInteractionUiViewStaticSelectElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "overflow"_L1) {
            AutoGenerateInteractionUiViewOverflowElement *e = new AutoGenerateInteractionUiViewOverflowElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "linear_scale"_L1) {
            AutoGenerateInteractionUiViewLinearScaleElement *e = new AutoGenerateInteractionUiViewLinearScaleElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "datepicker"_L1) {
            AutoGenerateInteractionUiViewDatePickerElement *e = new AutoGenerateInteractionUiViewDatePickerElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else if (type == "time_picker"_L1) {
            AutoGenerateInteractionUiViewTimePickerElement *e = new AutoGenerateInteractionUiViewTimePickerElement;
            e->parse(r.toObject());
            mElements.append(std::move(e));
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown elements type " << type;
        }
    }
}

QList<AutoGenerateInteractionUiViewActionable *> AutoGenerateInteractionUiViewActionsBlock::elements() const
{
    return mElements;
}

void AutoGenerateInteractionUiViewActionsBlock::setElements(const QList<AutoGenerateInteractionUiViewActionable *> &newElements)
{
    mElements = newElements;
}

QWidget *AutoGenerateInteractionUiViewActionsBlock::generateWidget(RocketChatAccount *account, QWidget *parent) const
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new FlowLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);
    for (const auto &e : std::as_const(mElements)) {
        connect(e, &AutoGenerateInteractionUiViewActionable::actionChanged, this, &AutoGenerateInteractionUiViewActionsBlock::actionChanged);
        hboxLayout->addWidget(e->generateWidget(account, parent));
    }
    // hboxLayout->addStretch(1);
    return widget;
}

void AutoGenerateInteractionUiViewActionsBlock::serializeBlock(QJsonObject &o) const
{
    QJsonArray array;
    for (const auto &e : mElements) {
        array.append(e->serialize());
    }
    o["elements"_L1] = array;
}
